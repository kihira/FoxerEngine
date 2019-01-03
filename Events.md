# Events
This engine implements an event system for communicating changes as well
as talking between subsystems. Events can be received and sent via Lua.

## Network
Events are supported over the network, with all current supported types
properly serialised as long as the correct event definition exists.

By default, all events generated on the client will be sent to the server
and all server events sent to ALL clients.

When an event is received on the server/client, the following argument is
set to allow for side detection
- `fromServer` *bool* client | server - Indicates where the packet
originated from

Two extra arguments are supported on events when used in a networked
environment:
- `noNetwork` *bool* client | server - Indicates that the event should not be
sent over the network. This will be set automatically by the server when
it receives an event from the client
- `sendToClient` *ClientId* server - Only send this event to a specific
client

## Default Argument Values
If trying to get an argument but the argument does not exist on the event,
a default value will be returned instead depending on the type:
- unsigned short: 0
- short: 0
- StringId: 0
- int: 0
- bool: false

## Events
#### EVENT_TYPE_LEVEL_UNLOAD
- *StringId* levelId - Level ID

#### EVENT_TYPE_LEVEL_LOAD
- *StringId* levelId - Level ID

#### EVENT_TYPE_ENTITY_SPAWN
Fired when an entity is spawned on the server side
- *StringId* prototypeId - ID of the prototype
- *EntityId* entityId - ID of the entity spawned

#### EVENT_TYPE_PLAYER_CONNECTED
- *ClientId* clientId - ID of the connected client

#### EVENT_TYPE_PLAYER_DISCONNECTED
- *ClientId* clientId - ID of the disconnect client

#### EVENT_TYPE_ENTITY_DESTROY
Fired AFTER an entity has been destroyed
- *EntityId* entityId - ID of the entity destroyed