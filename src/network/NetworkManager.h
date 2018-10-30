
#ifndef GAMEENGINE301CR_NETWORKMANAGER_H
#define GAMEENGINE301CR_NETWORKMANAGER_H


#include <enet/enet.h>
#include <map>

typedef void (* PacketHandlerFn)(int packetID, void *data, size_t dataLength);

struct PacketMeta {
    enet_uint8 id;
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
    static void packetFreeCallback(ENetPacket *packet);
    ENetPacket *buildPacket(PacketMeta meta, void *data, size_t dataLength);
public:
    void startUp();

    void shutDown();

    void startServer();
    void stopServer();
    void update();
    void registerPacket(PacketMeta meta);
    void sendToServer(enet_uint8 packetID, void *data, size_t dataLength);
    void sendToAllClients(enet_uint8 packetID, void *data, size_t dataLength);
    void connectToServer(const char *address, enet_uint16 port);
};


#endif //GAMEENGINE301CR_NETWORKMANAGER_H
