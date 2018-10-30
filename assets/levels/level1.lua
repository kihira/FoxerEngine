level = {
    name = "Test Level 1",
    players = 2,
    update = function()
    end
}

engine.registerEntityPrototype("entity", "testEntity")
engine.entity.spawn("Test");