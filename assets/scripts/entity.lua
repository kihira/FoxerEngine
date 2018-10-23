entity = {
    name = "Test",
    mesh = "doesnotexist",
    update = function()
        print("Hello, my name is " .. entity.name)
    end
}

function keyHandler(key, scancode, action, mods)
    print("Key! " .. key)
    return true;
end

engine.registerKeyHandler(keyHandler)
