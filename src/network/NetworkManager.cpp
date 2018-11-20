
#include <iostream>
#include <easy/profiler.h>
#include "../Managers.h"
#include "../assert.h"
#include "NetworkManager.h"


NetworkManager::NetworkManager() = default; // noop

NetworkManager::~NetworkManager() = default; // noop

void NetworkManager::startUp() {
    logger = spdlog::stdout_color_mt("network");

    if (enet_initialize() != 0) {
        logger->error("Failed to initialise ENet");
        exit(EXIT_FAILURE);
    }

    logger->info("ENet {0:d}.{0:d}.{0:d}", ENET_VERSION_MAJOR, ENET_VERSION_MINOR, ENET_VERSION_PATCH);

    // Register handler for network handshakes
    registerPacket({
        CLIENT_DATA_ID,
        0,
        ENET_PACKET_FLAG_RELIABLE,
        [](int packetID, void *data, size_t dataLength) {
            enet_uint8 clientId = (*((ClientData *)data)).clientId;
            gNetworkManager.clientId = clientId;
            spdlog::get("network")->debug("Client ID is: {0:d}", clientId);
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
        logger->error("Failed to start server on {0:d}:{0:d}", address.host, address.port);
        exit(EXIT_FAILURE);
    }
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
                    auto clientData = ClientData();
                    clientData.clientId = getNewClientId();
                    event.peer->data = &clientData;
                    clients[clientData.clientId] = event.peer;

                    // Tell client of its id
                    sendToClient(clientData.clientId, CLIENT_DATA_ID, &clientData, sizeof(ClientData));

                    logger->info("Client ({0:d}) connected to server", clientData.clientId);
                } else {
                    logger->info("Connected to server");
                }
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                enet_uint8 packetID = event.packet->data[0];
                packetHandlers[packetID].packetHandler(packetID, event.packet->data + sizeof(enet_uint8), event.packet->dataLength - sizeof(enet_uint8));
                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT:
                if (server) {
                    logger->info("Client ({0:d}) disconnected from server", ((ClientData *)event.peer)->clientId);
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
        logger->error("A packet with the ID {0:d} has already been registered!", meta.id);
        return;
    }

    ASSERT(meta.packetHandler != nullptr);
    ASSERT(meta.packetFlag > 0);
    ASSERT(meta.packetFlag < 4);

    packetHandlers.insert(std::pair<int, PacketMeta>(meta.id, meta));
}

void NetworkManager::sendToServer(enet_uint8 packetID, void *data, size_t dataLength) {
    ASSERT(!isServer());

    PacketMeta meta = packetHandlers[packetID];
    ENetPacket *packet = buildPacket(meta, data, dataLength);

    enet_peer_send(peer, meta.channel, packet);
}

void NetworkManager::sendToAllClients(enet_uint8 packetID, void *data, size_t dataLength) {
    ASSERT(isServer());

    PacketMeta meta = packetHandlers[packetID];
    ENetPacket *packet = buildPacket(meta, data, dataLength);

    enet_host_broadcast(host, meta.channel, packet);
}

void NetworkManager::sendToClient(enet_uint8 clientId, enet_uint8 packetID, void *data, size_t dataLength) {
    ASSERT(isServer());

    PacketMeta meta = packetHandlers[packetID];
    ENetPacket *packet = buildPacket(meta, data, dataLength);

    enet_peer_send(clients[clientId], meta.channel, packet);
}


void NetworkManager::packetFreeCallback(ENetPacket *packet) {
    free(packet->data);
}

void NetworkManager::connectToServer(const char *address, enet_uint16 port) {
    enet_address_set_host(&this->address, address);
    this->address.port = port;

    host = enet_host_create(nullptr, 1, 2, 0, 0);
    peer = enet_host_connect(host, &this->address, 2, 0);

    if (peer == nullptr) {
        logger->error("Failed to connect to server");
    }
}

ENetPacket *NetworkManager::buildPacket(PacketMeta meta, void *data, size_t dataLength) {
    // Append packet ID to the start of the packet so we know what to do with it on the other end
    void *newData = malloc(sizeof(enet_uint8) + dataLength);
    memcpy(newData, &meta.id, sizeof(enet_uint8));
    memcpy(static_cast<char*>(newData) + sizeof(enet_uint8), data, dataLength);
    dataLength += sizeof(enet_uint8);

    ENetPacket *packet = enet_packet_create(newData, dataLength, meta.packetFlag);
    packet->freeCallback = NetworkManager::packetFreeCallback;

    return packet;
}

bool NetworkManager::isServer() {
    return server;
}

enet_uint8 NetworkManager::getNewClientId() {
    lastClientId++;
    return lastClientId;
}