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
            auto event = Event((StringId) 0);
            event.deserialise(data);
            free(data);
        }
    });

    gEventManager.registerHandler(SID("EVENT_TYPE_INPUT_PLAYER"), this);
}

bool EventNetworkHandler::onEvent(Event &event) {
    if (gNetworkManager.isServer()) return false;

    void *data = nullptr;
    auto size = event.serialise(data);
    gNetworkManager.sendToServer(PACKET_ID_EVENT, data, size);
    free(data);

    return false;
}
