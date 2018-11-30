level = {
    name = "Main menu",
    players = 1,
    update = function(self)
    end
}

-- example of loading entity
-- local player = engine.entity.entity.new("testEntity")
local player = engine.entity.spawnEntity("testEntity")
player.name = "Player"
