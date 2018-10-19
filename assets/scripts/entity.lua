entity = {
    name = "Entity Name",
    mesh = "doesnotexist",
    update = function()
        print("Hello, my name is " .. entity.name)
    end
}

engine.addEntity(engine.entity.new("test"))