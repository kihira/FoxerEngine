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
            engine.graphics.camera.target = self.position
        end
    end,
    onSpawn = function(self)
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
                self.hasControl = true;
				self.controllingClient = event:getClientId("clientId")
            end
        elseif event:type() == 1770460267 then -- EVENT_TYPE_INPUT_PLAYER
            if (event:getEntityId("entityId") ~= self:id()) then return end
            if (self.hasControl ~= true) then return end
			-- if (self.controllingClient ~= event:getClientId("fromClient")) then return end

            -- Process input
            local inputBitmask = event:getUShort("inputBitmask");
            local physics = self:getPhysicsComponent();
            if (inputBitmask & 1 > 0) then
                local theta = -self.rotation.y;
                local cs = math.cos(theta)
                local sn = math.sin(theta)

                local xForce = 0
                local zForce = -1
                local force = engine.math.vec2.new(xForce * cs - zForce * sn, xForce * sn + zForce * cs)

                physics.velocity = physics.velocity + force
            end
            if (inputBitmask & 2 > 0) then
                local theta = -self.rotation.y;
                local cs = math.cos(theta)
                local sn = math.sin(theta)

                local xForce = 0
                local zForce = 1
                local force = engine.math.vec2.new(xForce * cs - zForce * sn, xForce * sn + zForce * cs)

                physics.velocity = physics.velocity + force
            end
            if (inputBitmask & 4 > 0) then
                physics.angularVelocity = physics.angularVelocity + 0.5;
                --physics.velocity = physics.velocity + engine.math.vec2.new(-1, 0)
            end
            if (inputBitmask & 8 > 0) then
                physics.angularVelocity = physics.angularVelocity + -0.5;
                -- physics.velocity = physics.velocity + engine.math.vec2.new(1, 0)
            end
        end
    end
}
