testEntity = {
    name = "Test Entity",
    physicsComponent = {
        type = 2, -- dynamic body
        gravityScale = 0.3
    },
    renderComponent = {
        mesh = "testMesh"
    },
    update = function(self)
        -- print("Hello, my name is " .. self.name)
    end,
    onSpawn = function(self)
        -- example of registering key handler
        local keyHandler = function(key, scancode, action, mods)
            if (key == 87) then --forward
                self.position.z = self.position.z + 0.1
            elseif (key == 83) then --backward
                self.position.z = self.position.z - 0.1
            elseif (key == 65) then --left
                self.position.x = self.position.x + 0.1
            elseif (key == 68) then --right
                self.position.x = self.position.x - 0.1
            end

        end
        engine.input.registerKeyHandler(keyHandler)
    end
}
