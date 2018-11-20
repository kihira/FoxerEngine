
#ifndef GAMEENGINE301CR_NETWORKMANAGER_H
#define GAMEENGINE301CR_NETWORKMANAGER_H


#include <enet/enet.h>
#include <map>

#define CLIENT_DATA_ID 1

typedef void (* PacketHandlerFn)(int packetID, void *data, size_t dataLength);

struct PacketMeta {
    enet_uint8 id;
    enet_uint8 channel;
    ENetPacketFlag packetFlag;
    PacketHandlerFn packetHandler;
};

struct ClientData {
    enet_uint8 clientId;
};

class NetworkManager {
private:
    ENetAddress address;
    ENetHost *host;
    ENetPeer *peer;
    bool server; // Whether we are a server or not
    enet_uint8 lastClientId = 1;
    std::map<enet_uint8, ENetPeer *> clients; // List of clients mapped between their peer and id
    std::map<enet_uint8, PacketMeta> packetHandlers;

    /**
     * Builds a ENet packet from the data provided so it is able to be sent
     * @param meta The packets meta information
     * @param data A pointer to the data to be sent
     * @param dataLength
     * @return The length of the data to be sent in bytes
     */
    ENetPacket *buildPacket(PacketMeta meta, void *data, size_t dataLength);

    /**
     * Returns a client ID that is currently not in use
     * @return
     */
    enet_uint8 getNewClientId();

    static void packetFreeCallback(ENetPacket *packet);
public:
    NetworkManager();

    ~NetworkManager();

    /**
     * Initialises the NetworkManager
     */
    void startUp();

    /**
     * De-initialises the NetworkManager
     */
    void shutDown();

    /**
     * Starts accepting connections and begins to act as a server
     */
    void startServer();

    /**
     * Disconnect connected clients and no longer accept incoming connections.
     */
    void stopServer();

    void update();

    /**
     * Registers a packet ID and it's handler so the packet can be properly routed to the correct handler
     * @param meta Description of the packet
     */
    void registerPacket(PacketMeta meta);

    /**
     * As a client, sends a packet to the currently connected server
     * @param packetID The ID of the packet
     * @param data A pointer to the data that should be sent
     * @param dataLength The length of the data to be sent in bytes
     */
    void sendToServer(enet_uint8 packetID, void *data, size_t dataLength);

    /**
     * As a server, sends a packet to all connected clients
     * @param packetID The ID of the packet
     * @param data A pointer to the data that should be sent
     * @param dataLength The length of the data to be sent in bytes
     */
    void sendToAllClients(enet_uint8 packetID, void *data, size_t dataLength);

    /**
     * As a server, sends a packet to the specific client
     * @param clientId The client id
     * @param data A pointer to the data that should be sent
     * @param dataLength The length of the data to be sent in bytes
     */
    void sendToClient(enet_uint8 clientId, enet_uint8 packetID, void *data, size_t dataLength);

    /**
     * As a client, attempts to connect to the provided address and port
     * @param address The address of the server
     * @param port The port of the server
     */
    void connectToServer(const char *address, enet_uint16 port);

    /**
     * Whether we are a server or not
     * @return
     */
    bool isServer();
};


#endif //GAMEENGINE301CR_NETWORKMANAGER_H
