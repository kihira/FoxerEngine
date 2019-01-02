return {
    name = "Player",
    position = engine.math.vec3.new(0.0),
    physicsComponent = {
        type = 2, -- dynamic body
        gravityScale = 0.3,
        linearDamping = 2,
        beginContact = function()
        end,
        endContact = function()
        end
    },
    renderComponent = {
        mesh = 591432690
    },
    update = function(self)
        -- print("Hello, my name is " .. self.name)
    end,
    onSpawn = function(self)
        engine.input.registerKeyHandler(function(key, scancode, action, mods)
            local inputBitmask = 0;
            if (key == 87) then --forward
                inputBitmask = inputBitmask + 1
            elseif (key == 83) then --backward
                inputBitmask = inputBitmask + 2
            elseif (key == 65) then --left
                inputBitmask = inputBitmask + 4
            elseif (key == 68) then --right
                inputBitmask = inputBitmask + 8
            end
            if (inputBitmask > 0) then
                local event = engine.event.event.new("EVENT_TYPE_INPUT_PLAYER")
                event:setEntityId("entityId", self:id())
                event:setUShort("inputBitmask", inputBitmask)
                event:push()
            end
        end)
    end,
    -- events to listen for
    events = {
        "EVENT_TYPE_ENTITY_SPAWN",
        "EVENT_TYPE_INPUT_PLAYER"
    },
    onEvent = function(self, event)
        if event:type() == 791803502 then -- EVENT_TYPE_ENTITY_SPAWN
            if event:getUShort("entityId") == self:id() then
                print("We've spawned!")
            end
        elseif event:type() == 1770460267 then -- EVENT_TYPE_INPUT_PLAYER
            if (not event:getEntityId("entityId") == self:id()) then return end

            local inputBitmask = event:getUShort("inputBitmask");
            if (inputBitmask & 1 > 0) then
                print("Forward!")
            end
            if (inputBitmask & 2 > 0) then
                print("Backward!")
            end
            if (inputBitmask & 4 > 0) then
                print("Left!")
            end
            if (inputBitmask & 8 > 0) then
                print("Right!")
            end
        end
    end
}
