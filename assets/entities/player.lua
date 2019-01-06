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
                    local theta = -self.rotation.y;
                    local cs = math.cos(theta)
                    local sn = math.sin(theta)
    
                    local xForce = 0
                    local zForce = -1
                    local force = engine.math.vec2.new(xForce * cs - zForce * sn, xForce * sn + zForce * cs)
    
                    physics.velocity = physics.velocity + force
                end
                if (self.inputBitmask & 2 == 2) then
                    local theta = -self.rotation.y;
                    local cs = math.cos(theta)
                    local sn = math.sin(theta)
    
                    local xForce = 0
                    local zForce = 1
                    local force = engine.math.vec2.new(xForce * cs - zForce * sn, xForce * sn + zForce * cs)
    
                    physics.velocity = physics.velocity + force
                end
                if (self.inputBitmask & 4 == 4) then
                    physics.angularVelocity = physics.angularVelocity + 0.05;
                end
                if (self.inputBitmask & 8 == 8) then
                    physics.angularVelocity = physics.angularVelocity + -0.05;
                end
            end

            -- chase camera
            local heading = engine.math.vec2.new(0, -30)
            local theta = -self.rotation.y;
            local cs = math.cos(theta)
            local sn = math.sin(theta)

            heading = engine.math.vec2.new(heading.x * cs - heading.y * sn, heading.x * sn + heading.y * cs)

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
        "EVENT_TYPE_INPUT_PLAYER"
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

                    print(self.inputBitmask)
                    
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
        end
    end
}
