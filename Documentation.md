# Foxer Engine Documentation
The Foxer engine relies heavily upon Lua scripts, allowing users to
create levels and entities (including logic) without needing to write
any C++ code. This document will serve as a description of how to write
these files as well as what data can be accessed and how to react to events.

## Namespaces/Subsystems
Every subsystem is accessible via the `engine` table with various methods to interact with them.

### Maths
`engine.maths`
#### Methods
```lua
vec2.new(float scaler)
vec2.new(float x, float y)
```
Returns: `engine.maths.vec2`

Creates a vector of 2 floats
***

```lua
vec3.new(float scaler)
vec3.new(float x, float y, float z)
```
Returns: `engine.maths.vec3`

Creates a vector of 3 floats
***

### Physics
`engine.physics`
#### Methods
```lua
setGravity(vec2 gravity)
```
Returns: `nil`

Sets the gravity value for the world. These values are in meters per second
***

```lua
getGravity()
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
spawnEntity(StringId prototypeId)
```
Returns: `engine.entity.entity`

Spawns an entity from the prototype