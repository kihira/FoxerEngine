
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

    // Append packet id to the data so we know what to do with it on the other end
    void *newData = (void *)malloc(sizeof(enet_uint8) + dataLength);
    memcpy(newData, &packetID, sizeof(enet_uint8));
    memcpy(&newData + sizeof(enet_uint8), data, dataLength);
    dataLength += sizeof(enet_uint8);

    ENetPacket *packet = enet_packet_create(newData, dataLength, meta.packetFlag);
    enet_peer_send(peer, meta.channel, packet);
    // enet_host_broadcast(host, packet); send to all connected peers
}
