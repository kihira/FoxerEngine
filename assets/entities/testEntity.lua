testEntity = {
    name = "Test Entity",
    collider = {
        gravityScale = 0.3
    },
    render = {
        mesh = "testMesh"
    },
    update = function(self)
        -- print("Hello, my name is " .. self.name)
    end,
    onSpawn = function(self)
        -- example of registering key handler
        local keyHandler = function(key, scancode, action, mods)
            print(key)
        end
        engine.input.registerKeyHandler(keyHandler)
    end
}
