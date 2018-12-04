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
        local keyHandler = function(key, scancode, action, mods)
            if (key == 87) then --forward
                self:getPhysicsComponent():setVelocity(engine.vec2.new(0.0, 1.0))
            elseif (key == 83) then --backward
                self:getPhysicsComponent():setVelocity(engine.vec2.new(0.0, -1.0))
            elseif (key == 65) then --left
                self:getPhysicsComponent():setVelocity(engine.vec2.new(1.0, 0.0))
            elseif (key == 68) then --right
                self:getPhysicsComponent():setVelocity(engine.vec2.new(-1.0, 0.0))
            end

        end
        engine.input.registerKeyHandler(keyHandler)
    end
}
