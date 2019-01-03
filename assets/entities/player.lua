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
    playerComponent = {},
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
    events = {
        "EVENT_TYPE_ASSIGN_PLAYER",
        "EVENT_TYPE_INPUT_PLAYER"
    },
    onEvent = function(self, event)
        if event:type() == 697357692 then -- EVENT_TYPE_ASSIGN_PLAYER
            if (not event:getEntityId("entityId") == self:id()) then return end

            -- If we're server or the client itself, say we have control
            if engine.network.isServer or (event:getClientId("clientId") == engine.network.clientId()) then
                print("We've been given control of an entity!")
                self.hasControl = true;
            end
        elseif event:type() == 1770460267 then -- EVENT_TYPE_INPUT_PLAYER
            if (event:getEntityId("entityId") ~= self:id()) then return end
            if (not self.hasControl) then return end

            print(event:getEntityId("entityId") .. " " .. self:id())
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
