
#include <iostream>
#include "NetworkManager.h"

NetworkManager::NetworkManager() {
    if (enet_initialize() != 0) {
        std::cerr << "Failed to initialise ENet" << std::endl;
        exit(-1);
    }
}

void NetworkManager::startServer() {
    address.host = ENET_HOST_ANY;
    address.port = 1234;

    server = enet_host_create(&address, 16, 2, 0, 0);

    if (server == nullptr) {
        std::cerr << "Failed to start server on " << address.host << ":" << address.port << std::endl;
        exit(-1);
    }
}

void NetworkManager::stopServer() {
    if (server == nullptr) {
        std::cerr << "Cannot stop server that is not running" << std::endl;
    }

    enet_host_destroy(server);
}

void NetworkManager::update() {
    ENetEvent event;
    while (enet_host_service(server, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                break;
            case ENET_EVENT_TYPE_NONE:
                break;
        }
    }
}

void NetworkManager::shutdown() {
    if (server != nullptr) {
        stopServer();
    }

    enet_deinitialize();
}
