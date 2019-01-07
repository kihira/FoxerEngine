# Foxer Engine Documentation
## Introduction
The Foxer engine relies heavily upon Lua scripts, allowing users to
create levels and entities (including logic) without needing to write
any C++ code. This document will serve as a description of how to write
these files as well as what data can be accessed and how to react to events.

## Definitions
To create an entity/level then a lua script needs to be created for that object to define it's properties. These are then placed in their respective folders and assigned to a StringId within the asset database.

### Entity
The entity definition supports numerous option for both default parameters (such as position) as well as ones that are just used during prototype construction.
#### Parameters
```lua
position = engine.math.vec3(0)
```
Defines the entities default position
***
```lua
rotation = engine.math.vec3(0)
```
Defines the entities default rotation in degrees. 
***
```lua
name = ""
```
Defines the entities default name
***
```lua
onSpawn = function(self)
```
A function that is called when the entity is spawned in the world (not when the prototype is created).\
`self` represents the entity that is currently being spawned.
***
```lua
update = function(self)
```
A function that is called every time the main game loop runs.\
`self` represents the entity that is currently being spawned.
***
```lua
events = {"", ""}
```
This should be an array of StringIds (in their String format) that defines what events `onEvent` should called for.
***
```lua
onEvent = function(self, event)
```
This is called every time an event is pushed into the queue and it's ID matches one defined in `events`.
`event` is an object of `engine.event.event` format.
***
#### Graphics Component
For something to be rendered on screen, the entity needs a graphics component. This can be done by creating the `renderComponent = {}` property and assigning it the following parameters
##### Parameters
```lua
mesh = 0
```
The StringID of the mesh for this component
***
```lua
shader = true
```
The StringID of the shader for this component
***
#### Physics Component
In order for the entity to be affected by physics, it needs a physics component to be registered on it. This can be done by creating the `physicsComponent = {}` property and assigning it the following parameters
##### Parameters
```lua
active = true
```
Whether the physics body should start as active
***
```lua
angularDamping = 0
```
The damping that should be applied to the angular velocity of the body (effectively acting as friction)
***
```lua
bullet = false
```
Whether the physics body should be treated as bullet. This does additional calculation steps as it expects that will be travelling as high speed
***
```lua
linearDamping = 0
```
The damping that should be applied to the velocity of the body (effectively acting as friction)
***
```lua
gravityScale = 1
```
How much the gravity should affect this object (if there is any)
***
```lua
type = 0
```
What type of physics body this is
- 0 Static: Body is not affected by forces and can only be moved by setting its position
- 1 Kinematic: Body is not affected by forces applied to it but can affect other bodies
- 2 Dynamic: Body is affected by forces and can affect other bodies
***
```lua
fixedRotation = false
```
Whether the rotation of the body is fixed
***
```lua
density = 0
```
The density of the object (repesented in kg)
***
```lua
friction = 0.2
```
The amount of friction the body has
***
```lua
restitution = 0
```
Represents how "bouncy" the object is
***
```lua
isSensor = false
```
Whether to treat the body as a sensor. If true, then the body doesn't effect other bodies but still calls `beginContact` and `endContact`
***
```lua
shape = 0
```
The shape of the phyics body
- 0 Circle: The body should be the shape of a circle. The following parameters are also accepted:\
`radius = 0.5` The radius of the circle
- 1 Rectangle: The body is the shape of a rectangle. The following parameters are also accepted:\
`centreX = 0` The centre of the body on the x axis
`centerZ = 0` The centre of the body on the z axis
`halfWidth = 0.5` Half of the length of the body on the x axis
`halfHeight = 0.5` Half of the length of the body on the z axis 
***
```lua
beginContact = function(self, other)
```
This function is called when this physics body is collides into another one.\
`self` represents this physics component.\
`other` represents the other physics component.
***
```lua
endContact = function(self, other)
```
This function is called when this physics body stops colliding into another one.\
`self` represents this physics component.\
`other` represents the other physics component.
***
#### Example
The following is an example of an entity with a physics component
```lua
return {
    name = "Entity",
    position = engine.math.vec3.new(0, 1, 0),
    physicsComponent = {
        type = 2,
        shape = 1,
        halfWidth = 1,
        halfHeight = 2
    }
}
```
## Namespaces/Subsystems
Every subsystem is accessible via the `engine` table with various methods to interact with them.

### Maths
`engine.maths`
#### Methods
```lua
engine.maths.vec2.new(float scaler)
engine.maths.vec2.new(float x, float y)
```
Returns: `engine.maths.vec2`

Creates a vector of 2 floats
***

```lua
engine.maths.vec3.new(float scaler)
engine.maths.vec3.new(float x, float y, float z)
```
Returns: `engine.maths.vec3`

Creates a vector of 3 floats
***

```lua
engine.maths.rotateVec2(engine.math.vec2 vector, float rotation)
```
Returns: `engine.maths.vec2`

Rotates the provided vector by the rotation and returns a new vector with the result
***

### Physics
`engine.physics`
#### Methods
```lua
engine.physics.setGravity(vec2 gravity)
```
Returns: `nil`

Sets the gravity value for the world. These values are in meters per second
***

```lua
engine.physics.getGravity()
```
Returns: `engine.maths.vec2`

Gets the gravity value for the world. These values are in meters per second
***
#### Example
Setting the gravity for the world to match Earths gravity on the x axis

```lua
engine.physics:setGravity(engine.math.vec2.new(9.8, 0))
```

### Entity
`engine.entity`
#### Methods
```lua
engine.entity.spawnEntity(StringId prototypeId)
```
Returns: `engine.entity.entity`

Spawns an entity from the prototype
***

```lua
engine.entity.getEntity(StringId entityId)
```
Returns: `engine.entity.entity`

Gets an entity by its ID, otherwise returns `nil` if not found
***

```lua
engine.entity.destroy(StringId entityId)
```
Returns: `nil`

Destroys the entity with that ID. If none exist with that ID, an error will be logged
***

### Event
`engine.event`
#### Methods
```lua
engine.event.push(Event event)
```
Returns: `nil`

Pushes an event into the event queue to be resolved immediately
***

### Network
`engine.network`
#### Methods
```lua
engine.network.isServer()
```
Returns: `bool`

True is the game is currently running as a server
***

```lua
engine.network.isClient()
```
Returns: `bool`

True is the game is currently running as a client
***

```lua
engine.network.clientsCount()
```
Returns: `unsigned short`

Gets the number of connected clients to the server. Returns 0 if not a server
***

```lua
engine.network.clientId()
```
Returns: `unsigned short`

Gets the current client ID if the game is a client, otherwise 0
***

```lua
engine.network.clientsCount()
```
Returns: `array<unsigned short>`

Gets all the connected clients IDs and returns them in an array format. If not a server, returns `nil`
***

### Input
`engine.input`
#### Methods
```lua
engine.input.registerKeyHandler(function callback)
```
Returns: `nil`

Registers a function that will be called every time a key is pressed, released or repeated. The function should take the following format:
```lua
function callback(key, scancode, action, mods)
```
`key` is the numeric value of the key (not the character) that is being pressed. The values for these keys are found [here](https://www.glfw.org/docs/latest/group__keys.html).

`scancode` is a platform specific, unique value for the key. A list is not available as it varies per platform but it is consistent over time

`action` represents what the key did and is one of the following values:
- 0 Key Released
- 1 Key Pressed
- 2 Key Repeated

The key repeated does not mean that every frame it will be called as how often a key "repeats" is dependant upon the operating system.

`mods` is a bitflag of what additional keys are pressed when the event fires. The following bits will be set:
- 0x0001 Shift key
- 0x0002 Ctrl key
- 0x0004 Alt key
- 0x0008 Super (aka Windows or Cmd) key

The check if a bit is set, you will need to use a bitwise AND operator like so:
```lua
function callback(key, scancode, action, mods) then
    if 1 & mods == 1 then
        -- shift key pressed
    end
    if 4 & mods == 4 then
        -- alt key is pressed
end
```

If the keyboard has multiple modifier keys, then the flag will be set if any of them are pressed.
***

```lua
engine.input.registerCursorHandler(function callback)
```
Returns: `nil`

Registers a function that will be called every time the cursor is updated via movement. The function should take the following format:
```lua
function callback(xPos, yPos)
```
`xPos` and `yPos` are the screen coordinate position of the cursor at the time of the update. The coordinates are relative to the top-left corner of the window and supports sub-pixel position
***