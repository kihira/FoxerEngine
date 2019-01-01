
#include <iostream>
#include <easy/profiler.h>
#include "../Managers.h"
#include "../util/assert.h"
#include "NetworkManager.h"
#include "../event/Event.h"


NetworkManager::NetworkManager() = default; // noop

NetworkManager::~NetworkManager() = default; // noop

void NetworkManager::startUp() {
    logger = spdlog::get("main")->clone("network");
    spdlog::register_logger(logger);

    if (enet_initialize() != 0) {
        logger->error("Failed to initialise ENet");
        exit(EXIT_FAILURE);
    }

    logger->info("ENet {:d}.{:d}.{:d}", ENET_VERSION_MAJOR, ENET_VERSION_MINOR, ENET_VERSION_PATCH);

    auto event = Event(SID("EVENT_TYPE_NETWORK_STARTUP"));
    event.push();

    // Register handler for network handshakes
    registerPacket(
            {
                    PACKET_ID_CLIENT_DATA,
                    0,
                    ENET_PACKET_FLAG_RELIABLE,
                    [](int packetID, void *data, size_t dataLength) {
                        ClientId clientId = static_cast<ClientData *>(data)->clientId;
                        gNetworkManager.clientId = clientId;
                        spdlog::get("network")->debug("Client ID is: {:d}", clientId);
                    }
            });
}

void NetworkManager::shutDown() {
    if (host != nullptr) {
        stopServer();
    }

    enet_deinitialize();
}

void NetworkManager::startServer() {
    address.host = ENET_HOST_ANY;
    address.port = 1234;

    host = enet_host_create(&address, 16, 2, 0, 0);
    server = true;

    if (host == nullptr) {
        logger->error("Failed to start server on {:d}:{:d}", address.host, address.port);
        exit(EXIT_FAILURE);
    }

    auto event = Event(SID("EVENT_TYPE_SERVER_START"));
    event.push();
}

void NetworkManager::stopServer() {
    if (host == nullptr) {
        logger->warn("Cannot stop server that is not running");
    }

    enet_host_destroy(host);
}

void NetworkManager::update() {
    EASY_FUNCTION();
    ENetEvent event;
    while (enet_host_service(host, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                if (server) {
                    // Generate client id and store it
                    auto clientData = new ClientData();
                    clientData->clientId = getNewClientId();
                    event.peer->data = clientData;
                    clients[clientData->clientId] = event.peer;

                    // Tell client of its id
                    sendToClient(clientData->clientId, PACKET_ID_CLIENT_DATA, clientData, sizeof(ClientData));

                    // Post event
                    auto event = Event(SID("EVENT_TYPE_PLAYER_CONNECTED"));
                    event.setArg("clientId", clientData->clientId);
                    event.push();

                    logger->info("Client ({:d}) connected to server", clientData->clientId);
                } else {
                    logger->info("Connected to server");
                }
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                enet_uint8 packetID = event.packet->data[0];
                packetHandlers[packetID].packetHandler(packetID, event.packet->data + sizeof(enet_uint8),
                                                       event.packet->dataLength - sizeof(enet_uint8));
                logger->debug("Received packet. ID: {:d} Length: {:d}", packetID, event.packet->dataLength);
                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT:
                if (server) {
                    // Remove client data
                    auto clientId = static_cast<ClientData *>(event.peer->data)->clientId;
                    delete clients[clientId];
                    clients.erase(clientId);

                    // Post event
                    auto event = Event(SID("EVENT_TYPE_PLAYER_DISCONNECTED"));
                    event.setArg("clientId", clientId);
                    event.push();

                    logger->info("Client ({:d}) disconnected from server", clientId);
                } else {
                    logger->info("Disconnected from server");
                    peer = nullptr;
                }
                break;
            case ENET_EVENT_TYPE_NONE:
                break;
        }
    }
}

void NetworkManager::registerPacket(PacketMeta meta) {
    if (packetHandlers.find(meta.id) != packetHandlers.end()) {
        logger->error("A packet with the ID {:d} has already been registered!", meta.id);
        return;
    }

    ASSERT(meta.packetHandler != nullptr);
    ASSERT(meta.packetFlag > 0);
    ASSERT(meta.packetFlag < 4);

    packetHandlers.insert(std::pair<int, PacketMeta>(meta.id, meta));
    logger->debug("Register packet handler. ID: {:d}", meta.id);
}

void NetworkManager::sendToServer(PacketId packetID, void *data, size_t dataLength) {
    ASSERT(!isServer());

    PacketMeta meta = packetHandlers[packetID];
    ENetPacket *packet = buildPacket(meta, data, dataLength);

    enet_peer_send(peer, meta.channel, packet);
}

void NetworkManager::sendToAllClients(PacketId packetID, void *data, size_t dataLength) {
    ASSERT(isServer());

    PacketMeta meta = packetHandlers[packetID];
    ENetPacket *packet = buildPacket(meta, data, dataLength);

    enet_host_broadcast(host, meta.channel, packet);
}

void NetworkManager::sendToClient(ClientId clientId, PacketId packetID, void *data, size_t dataLength) {
    ASSERT(isServer());

    PacketMeta meta = packetHandlers[packetID];
    ENetPacket *packet = buildPacket(meta, data, dataLength);

    enet_peer_send(clients[clientId], meta.channel, packet);
}


void NetworkManager::packetFreeCallback(ENetPacket *packet) {
    free(packet->data);
    packet->data = nullptr;
}

void NetworkManager::connectToServer(const char *address, enet_uint16 port) {
    enet_address_set_host(&this->address, address);
    this->address.port = port;

    auto event = Event(SID("EVENT_TYPE_CLIENT_START"));
    event.push();

    host = enet_host_create(nullptr, 1, 2, 0, 0);
    peer = enet_host_connect(host, &this->address, 2, 0);

    if (peer == nullptr) {
        logger->error("Failed to connect to server");
    }
}

ENetPacket *NetworkManager::buildPacket(PacketMeta meta, void *data, size_t dataLength) {
    // Append packet ID to the start of the packet so we know what to do with it on the other end
    void *newData = malloc(sizeof(PacketId) + dataLength);
    memcpy(newData, &meta.id, sizeof(PacketId));
    memcpy(static_cast<char *>(newData) + sizeof(PacketId), data, dataLength);
    dataLength += sizeof(PacketId);

    ENetPacket *packet = enet_packet_create(newData, dataLength, meta.packetFlag);
    packet->freeCallback = NetworkManager::packetFreeCallback;

    return packet;
}

bool NetworkManager::isServer() {
    return server;
}

ClientId NetworkManager::getNewClientId() {
    lastClientId++;
    return lastClientId;
}

unsigned long NetworkManager::clientsCount() {
    return clients.size();
}
