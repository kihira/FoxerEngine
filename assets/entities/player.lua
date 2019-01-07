return {
    name = "Player",
    position = engine.math.vec3.new(0, 2, 0),
    physicsComponent = {
        type = 2, -- dynamic body
        gravityScale = 0.3,
        linearDamping = 1,
        angularDamping = 3,
		shape = 1,
		halfWidth = 3,
		halfHeight = 8,
    },
    renderComponent = {
        mesh = 3415080175,
		shader = 2946951896
    },
    networkComponent = {
        syncRate = 1 / 10
    },
    playerComponent = {},
    update = function(self)
        if (self.hasControl) then
            -- apply input
            if (self.inputBitmask > 0) then
                local physics = self:getPhysicsComponent();
                if (self.inputBitmask & 1 == 1) then
                    local force = engine.math.rotateVec2(engine.math.vec2.new(0, -1), -self.rotation.y)
                    physics.velocity = physics.velocity + force
                end
                if (self.inputBitmask & 2 == 2) then
                    local force = engine.math.rotateVec2(engine.math.vec2.new(0, 1), -self.rotation.y)
                    physics.velocity = physics.velocity + force
                end
                if (self.inputBitmask & 4 == 4) then
                    physics.angularVelocity = physics.angularVelocity - 0.0025;
                end
                if (self.inputBitmask & 8 == 8) then
                    physics.angularVelocity = physics.angularVelocity + 0.0025;
                end
            end

            -- chase camera
            local heading = engine.math.rotateVec2(engine.math.vec2.new(0, -30), -self.rotation.y)

            local camPosition = engine.graphics.camera.position
            camPosition.x = self.position.x + -heading.x
            camPosition.y = self.position.y + 12
            camPosition.z = self.position.z + -heading.y
            engine.graphics.camera.position = camPosition

            local camTarget = engine.graphics.camera.target
            camTarget.x = self.position.x + heading.x
            camTarget.y = 0
            camTarget.z = self.position.z + heading.y
            engine.graphics.camera.target = camTarget
        end
    end,
    onSpawn = function(self)
        self.inputBitmask = 0
    end,
    events = {
        "EVENT_TYPE_ASSIGN_PLAYER",
        "EVENT_TYPE_INPUT_PLAYER",
        "EVENT_TYPE_PLAYER_WIN"
    },
    onEvent = function(self, event)
        if event:type() == 697357692 then -- EVENT_TYPE_ASSIGN_PLAYER
            if (event:getEntityId("entityId") ~= self:id()) then return end
            -- If we're server or the client itself, we have control
            if engine.network.isServer() or (event:getClientId("clientId") == engine.network.clientId()) then
				-- as we have control, can listen for inputs
                engine.input.registerKeyHandler(function(key, scancode, action, mods)
                    if (action > 1) then return end -- only listen for press and release

                    local modifier = 1
                    if (action == 0) then modifier = -1 end

					if (key == 87) then --forward
                        self.inputBitmask = self.inputBitmask + 1 * modifier
                    elseif (key == 83) then --backward
                        self.inputBitmask = self.inputBitmask + 2 * modifier
                    end
					if (key == 65) then --left
                        self.inputBitmask = self.inputBitmask + 4 * modifier
                    end
					if (key == 68) then --right
						self.inputBitmask = self.inputBitmask + 8 * modifier
                    end
                    
                    -- Only send event if something has changed
                    local event = engine.event.event.new("EVENT_TYPE_INPUT_PLAYER")
                    event:setEntityId("entityId", self:id())
                    event:setUShort("inputBitmask", self.inputBitmask)
                    event:push()
				end)
                self.hasControl = true;
				self.controllingClient = event:getClientId("clientId")
            end
        elseif event:type() == 1770460267 then -- EVENT_TYPE_INPUT_PLAYER
            if (event:getEntityId("entityId") ~= self:id()) then return end
            if (self.hasControl ~= true) then return end

            -- Process input
            self.inputBitmask = event:getUShort("inputBitmask");
        elseif event:type() == 3655313229 then -- EVENT_TYPE_PLAYER_WIN
            if engine.network.isServer() then return end
            if self.hasControl == nil then return end

            if event:getEntityId("entityId") == self:id() then
                engine.graphics.camera.position = engine.math.vec3.new(0, 900, 10)
                engine.graphics.camera.target = engine.math.vec3.new(0, 900, 0)
            else
                engine.graphics.camera.position = engine.math.vec3.new(0, 800, 10)
                engine.graphics.camera.target = engine.math.vec3.new(0, 800, 0)
            end

            self.hasControl = false
        end
    end
}
