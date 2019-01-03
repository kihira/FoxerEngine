
#ifndef GAMEENGINE301CR_NETWORKMANAGER_H
#define GAMEENGINE301CR_NETWORKMANAGER_H


#include <enet/enet.h>
#include <spdlog/spdlog.h>
#include <map>

#define PACKET_ID_CLIENT_DATA 1

typedef enet_uint16 ClientId;
typedef enet_uint8 PacketId;
typedef void (* PacketHandlerFn)(PacketId packetID, void *data, size_t dataLength);

struct PacketMeta {
    PacketId id;
    enet_uint8 channel;
    ENetPacketFlag packetFlag;
    PacketHandlerFn packetHandler;
};

struct ClientData {
    ClientId clientId;
};

class NetworkManager {
private:
    ENetAddress address;
    ENetHost *host;
    ENetPeer *peer;
    std::shared_ptr<spdlog::logger> logger;
    std::map<PacketId, PacketMeta> packetHandlers;

    // Server only data
    bool server; // Whether we are a server or not
    ClientId lastClientId = 0;
    std::map<ClientId, ENetPeer *> clients; // List of clients mapped between their peer and id

    // Client only data
    ClientId clientId;

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
    ClientId getNewClientId();

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
    void sendToServer(PacketId packetID, void *data, size_t dataLength);

    /**
     * As a server, sends a packet to all connected clients
     * @param packetID The ID of the packet
     * @param data A pointer to the data that should be sent
     * @param dataLength The length of the data to be sent in bytes
     */
    void sendToAllClients(PacketId packetID, void *data, size_t dataLength);

    /**
     * As a server, sends a packet to the specific client
     * @param clientId The client id
     * @param data A pointer to the data that should be sent
     * @param dataLength The length of the data to be sent in bytes
     */
    void sendToClient(ClientId clientId, PacketId packetID, void *data, size_t dataLength);

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

    /**
     * The number of connected clients to the server.
     * Returns 0 if we're a client
     * @return Number of connected clients, otherwise 0 if client
     */
    unsigned long clientsCount();

    /**
     * If this is a client, return it's assigned client ID, otherwise 0
     * @return The client Id
     */
    ClientId getClientId() const;
};


#endif //GAMEENGINE301CR_NETWORKMANAGER_H
