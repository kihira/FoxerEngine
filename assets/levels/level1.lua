level = {
    name = "Test Level 1",
    players = 2,
    update = function()
    end
}

-- example of loading entity
local player = engine.spawnEntity("player")
player.name = "Player"