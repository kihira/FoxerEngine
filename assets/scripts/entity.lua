testEntity = {
    name = "Test",
    mesh = "doesnotexist",
    update = function(entity)
        print("Hello, my name is " .. entity.name)
    end
}
