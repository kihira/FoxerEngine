
#ifndef GAMEENGINE301CR_NETWORKMANAGER_H
#define GAMEENGINE301CR_NETWORKMANAGER_H


#include <enet/enet.h>
#include <map>

typedef void (* PacketHandlerFn)(int packetID, void *data, size_t dataLength);

struct PacketMeta {
    enet_uint8 channel;
    ENetPacketFlag packetFlag;
    PacketHandlerFn packetHandler;
};

class NetworkManager {
private:
    ENetAddress address;
    ENetHost *host;
    ENetPeer *peer;
    std::map<enet_uint8, PacketMeta> packetHandlers;
public:
    NetworkManager();
    void startServer();
    void stopServer();
    void update();
    void shutdown();
    void registerPacket(enet_uint8 packetID, PacketMeta meta);
    void sendToServer(enet_uint8 packetID, void *data, size_t dataLength);
};


#endif //GAMEENGINE301CR_NETWORKMANAGER_H
