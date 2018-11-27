level = {
    name = "Main menu",
    players = 1,
    update = function(self)
    end
}

-- example of loading entity
local player = engine.entity.spawnEntity("dummyTarget")
player.name = "Player"

-- example of registering key handler
local keyHandler = function(key, scancode, action, mods)
    print(key)
end

engine.input.registerKeyHandler(keyHandler)