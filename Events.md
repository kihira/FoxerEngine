# Events
This engine implements an event system for communicating changes as well
as talking between subsystems. Events can be received and sent via Lua.

## Network
Events are supported over the network, with all current supported types
properly serialised as long as the correct event definition exists.

By default, all events generated on the client will be sent to the server
and all server events sent to ALL clients.

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
*server*
- *StringId* levelId - Level ID

#### EVENT_TYPE_LEVEL_LOAD
*server*
- *StringId* levelId - Level ID

#### EVENT_TYPE_ENTITY_SPAWNED
*client*
- *unsigned short* entityId - ID of the entity spawned

#### EVENT_TYPE_PLAYER_CONNECTED
- *unsigned short* clientId - ID of the connected client