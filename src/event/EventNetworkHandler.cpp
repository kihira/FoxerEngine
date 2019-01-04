#include "EventNetworkHandler.h"
#include "../Managers.h"
#include "../network/NetworkManager.h"
#include "EventManager.h"

#define PACKET_ID_EVENT 5

EventNetworkHandler::EventNetworkHandler() {
    gNetworkManager.registerPacket({
        PACKET_ID_EVENT,
        0,
        static_cast<ENetPacketFlag>(0),
        [](PacketId packetId, void *data, size_t dataLength) {
            // Parse and deserialise event data
            auto event = Event((StringId) 0); // Should really not do this, bad practice
            event.deserialise(data);
            event.setArg("noNetwork", true); // Don't want an infinite loop
            event.setArg("fromServer", !gNetworkManager.isServer());
            event.push();
        }
    });

    gEventManager.registerHandler(this);
}

bool EventNetworkHandler::onEvent(Event &event) {
    if (event.getArg<bool>("noNetwork")) return false;

    // Serialise event
    void *data = nullptr;
    auto size = event.serialise(data);

    // Send to server/client(s)
    if (gNetworkManager.isServer()) {
        auto clientId = event.getArg<ClientId>("sendToClient");
        if (clientId > 0) {
            gNetworkManager.sendToClient(clientId, PACKET_ID_EVENT, data, size);
			free(data);
			return true;
        } else {
            gNetworkManager.sendToAllClients(PACKET_ID_EVENT, data, size);
        }
    } else {
        gNetworkManager.sendToServer(PACKET_ID_EVENT, data, size);
    }

    // Cleanup
    free(data);
    return false;
}
