level = {
    name = "Test Level 1",
    players = 2,
    update = function(self)
    end,
    load = function(self)
    end
}

-- example of loading entity
local player = engine.spawnEntity("dummyTarget")
player.name = "Player"