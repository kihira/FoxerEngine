
#include <iostream>
#include "NetworkManager.h"

NetworkManager::NetworkManager() {
    if (enet_initialize() != 0) {
        std::cerr << "Failed to initialise ENet" << std::endl;
        exit(-1);
    }

    std::cout << "ENet " << ENET_VERSION_MAJOR << "." << ENET_VERSION_MINOR << "." << ENET_VERSION_PATCH << std::endl;
}

void NetworkManager::startServer() {
    address.host = ENET_HOST_ANY;
    address.port = 1234;

    host = enet_host_create(&address, 16, 2, 0, 0);

    if (host == nullptr) {
        std::cerr << "Failed to start server on " << address.host << ":" << address.port << std::endl;
        exit(-1);
    }
}

void NetworkManager::stopServer() {
    if (host == nullptr) {
        std::cerr << "Cannot stop server that is not running" << std::endl;
    }

    enet_host_destroy(host);
}

void NetworkManager::update() {
    ENetEvent event;
    while (enet_host_service(host, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                peer = event.peer;
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                enet_uint8 packetID = event.packet->data[0];
                packetHandlers[packetID].packetHandler(packetID, &event.packet->data[1], event.packet->dataLength - sizeof(enet_uint8));
                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT:
                peer = nullptr;
                break;
            case ENET_EVENT_TYPE_NONE:
                break;
        }
    }
}

void NetworkManager::shutdown() {
    if (host != nullptr) {
        stopServer();
    }

    enet_deinitialize();
}

void NetworkManager::registerPacket(enet_uint8 packetID, PacketMeta meta) {
    if (packetHandlers.find(packetID) != packetHandlers.end()) {
        std::cerr << "A packet with the ID " << packetID << " has already been registered!" << std::endl;
        return;
    }

    packetHandlers.insert(std::pair<int, PacketMeta>(packetID, meta));
}

void NetworkManager::sendToServer(enet_uint8 packetID, void *data, size_t dataLength) {
    PacketMeta meta = packetHandlers[packetID];
    ENetPacket *packet = buildPacket(meta, data, dataLength);

    enet_peer_send(peer, meta.channel, packet);
}

void NetworkManager::sendToAllClients(enet_uint8 packetID, void *data, size_t dataLength) {
    PacketMeta meta = packetHandlers[packetID];
    ENetPacket *packet = buildPacket(meta, data, dataLength);

    enet_host_broadcast(host, meta.channel, packet);
}

void NetworkManager::packetFreeCallback(ENetPacket *packet) {
    free(packet->data);
}

void NetworkManager::connectToServer(const char *address, enet_uint16 port) {
    enet_address_set_host(&this->address, address);
    this->address.port = port;

    peer = enet_host_connect(host, &this->address, 2, 0);

    if (peer == nullptr) {
        std::cerr << "Failed to connect to server" << std::endl;
    }
}

ENetPacket *NetworkManager::buildPacket(PacketMeta meta, void *data, size_t dataLength) {
    // Append packet ID to the start of the packet so we know what to do with it on the other end
    void *newData = malloc(sizeof(enet_uint8) + dataLength);
    memcpy(newData, &meta.id, sizeof(enet_uint8));
    memcpy(&newData + sizeof(enet_uint8), data, dataLength);
    dataLength += sizeof(enet_uint8);

    ENetPacket *packet = enet_packet_create(newData, dataLength, meta.packetFlag);
    packet->freeCallback = NetworkManager::packetFreeCallback;

    return packet;
}