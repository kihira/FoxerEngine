entity = {
    name = "Entity Name",
    mesh = "doesnotexist",
    update = function()
        print("Hello, my name is " .. entity.name)
    end
}

newEntity = engine.entity.new("test")
newEntity:loadScript("assets/scripts/entity2.lua")
engine.addEntity(newEntity)