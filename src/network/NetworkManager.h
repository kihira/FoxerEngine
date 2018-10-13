
#ifndef GAMEENGINE301CR_NETWORKMANAGER_H
#define GAMEENGINE301CR_NETWORKMANAGER_H


#include <enet/enet.h>

class NetworkManager {
private:
    ENetAddress address;
    ENetHost *server;
public:
    NetworkManager();
    void startServer();
    void stopServer();
    void update();
    void shutdown();
};


#endif //GAMEENGINE301CR_NETWORKMANAGER_H
