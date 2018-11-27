level = {
    name = "Main menu",
    players = 1,
    update = function(self)
    end
}

-- example of loading entity
local player = engine.entity.spawnEntity("testEntity")
player.name = "Player"

-- example of registering key handler
local keyHandler = function(key, scancode, action, mods)
    print(key)
end
engine.input.registerKeyHandler(keyHandler)

-- example of registering cursor handler
local cursorHandler = function(xPos, yPos)
    print(xPos .. " " .. yPos)
end
engine.input.registerCursorHandler(cursorHandler)