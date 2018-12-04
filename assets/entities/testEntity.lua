testEntity = {
    name = "Test Entity",
    physicsComponent = {
        type = 2, -- dynamic body
        gravityScale = 0.3,
        beginContact = function()
        end,
        endContact = function()
        end
    },
    renderComponent = {
        mesh = "testMesh"
    },
    update = function(self)
        -- print("Hello, my name is " .. self.name)
    end,
    onSpawn = function(self)
        -- example of registering key handler
        local physics = self:getPhysicsComponent()
        local keyHandler = function(key, scancode, action, mods)
            if (key == 87) then --forward
                physics.velocity = physics.velocity + engine.math.vec2.new(0.0, 1.0)
            elseif (key == 83) then --backward
                physics.velocity = physics.velocity + engine.math.vec2.new(0.0, -1.0)
            elseif (key == 65) then --left
                physics.velocity = physics.velocity + engine.math.vec2.new(1.0, 0.0)
            elseif (key == 68) then --right
                physics.velocity = physics.velocity + engine.math.vec2.new(-1.0, 0.0)
            end

        end
        engine.input.registerKeyHandler(keyHandler)
    end
}
