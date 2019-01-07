level = {
    id = 248972157, --- LEVEL_1
    name = "Level 1",
    players = 2,
    entities = {
        {
            prototypeId = 315271780,
            entityId = 10000,
            position = engine.math.vec3.new(0),
            rotation = engine.math.vec3.new(0)
        },
		{
            prototypeId = 315271780,
            entityId = 10001,
            position = engine.math.vec3.new(10, 0, 0),
            rotation = engine.math.vec3.new(0)
        },
		{
            prototypeId = 315271780,
            entityId = 10002,
            position = engine.math.vec3.new(10, 0, 10),
            rotation = engine.math.vec3.new(0)
        },
        {
            prototypeId = 315271780,
            entityId = 10003,
            position = engine.math.vec3.new(0, 0, 10),
            rotation = engine.math.vec3.new(0)
        },
        {
            prototypeId = 315271780,
            entityId = 10004,
            position = engine.math.vec3.new(-10, 0, 0),
            rotation = engine.math.vec3.new(0)
        },
		{
            prototypeId = 315271780,
            entityId = 10005,
            position = engine.math.vec3.new(-10, 0, -10),
            rotation = engine.math.vec3.new(0)
        },
        {
            prototypeId = 315271780,
            entityId = 10006,
            position = engine.math.vec3.new(0, 0, -10),
            rotation = engine.math.vec3.new(0)
        },
        {
            prototypeId = 315271780,
            entityId = 10007,
            position = engine.math.vec3.new(10, 0, -10),
            rotation = engine.math.vec3.new(0)
        },
        {
            prototypeId = 315271780,
            entityId = 10008,
            position = engine.math.vec3.new(-10, 0, 10),
            rotation = engine.math.vec3.new(0)
        },
		-- {
        --     prototypeId = 1675181212,
        --     entityId = 10200,
        --     position = engine.math.vec3.new(0, 0, 10),
        --     rotation = engine.math.vec3.new(0)
        -- },
        {
            prototypeId = 592045845, -- ENTITY_PORTAL
            entityId = 10100,
            position = engine.math.vec3.new(0, 0, -50),
            rotation = engine.math.vec3.new(0)
        },
        {
            prototypeId = 592045845, -- ENTITY_PORTAL
            entityId = 10101,
            position = engine.math.vec3.new(30, 0, -150),
            rotation = engine.math.vec3.new(0, 30, 0)
        },
        {
            prototypeId = 592045845, -- ENTITY_PORTAL
            entityId = 10103,
            position = engine.math.vec3.new(100, 0, -250),
            rotation = engine.math.vec3.new(0, 90, 0)
        }
    },
    update = function(self)
        if (self.hasPlayerWon) then return end

        for k, v in pairs(self.portalsCompleted) do
            local count = 0
            for k1, v1 in pairs(v) do
                count = count + 1
            end

            -- check if all portals have been completed
            if (count == 3) then
                print("Entity " .. k .. " has completed all portals and wins!")

                self.hasPlayerWon = true
                local event = engine.event.event.new("EVENT_TYPE_PLAYER_WIN")
                event:setEntityId("entityId", k)
                event:push()
            end
        end
    end,
    events = {
        "EVENT_TYPE_LEVEL_LOAD",
        "EVENT_TYPE_PLAYER_CONNECTED",
        "EVENT_TYPE_PLAYER_DISCONNECTED",
        "EVENT_TYPE_PLAYER_WIN"
    },
    onEvent = function(self, event)
        if (event:type() == 1994444546) then -- EVENT_TYPE_PLAYER_CONNECTED
            print("Player connected! Client ID: " .. event:getUShort("clientId"))

			-- Wait until we have two players connected before starting
			if (engine.network.clientsCount() == 2) then
				for i = 1, 2, 1 do
					-- Spawn player entity and assign it
					local player = engine.entity.spawnEntity("ENTITY_PLAYER")
					local clientId = engine.network.getClientIds()[i]
                    player.controllingClient = clientId
                    player.position = engine.math.vec3.new(-20 + ((i-1)*40), 4, 0);

                    -- store player id in portals table so we can track
                    self.portalsCompleted[player:id()] = {}

					-- Send event out so client know its can control it
					local assignEvent = engine.event.event.new("EVENT_TYPE_ASSIGN_PLAYER")
					assignEvent:setClientId("clientId", clientId)
					assignEvent:setEntityId("entityId", player:id())
					assignEvent:push()
				end
			end

        elseif event:type() == 415743068 then -- EVENT_TYPE_PLAYER_DISCONNECTED
            local clientId = event:getClientId("clientId")
            print("Client " .. clientId .. " disconnected")

            -- Ask engine to destroy and remove our reference
            -- engine.entity.destroy(self.players[clientId]:id())
            -- self.players[clientId] = nil

        elseif event:type() == 1205121214 then -- EVENT_TYPE_LEVEL_LOAD
            if (event:getStringId("levelId") == self:id()) then
                print("Level loaded!")

                -- setup some data structures
                self.portalsCompleted = {}
                self.portalHit = function(portalId, entityId)
                    local portalsCompleted = self.portalsCompleted[entityId]
                    -- If haven't hit this portal already, store it
                    if (self.portalsCompleted[entityId][portalId] == nil) then
                        self.portalsCompleted[entityId][portalId] = true
                    end
                end
                self.hasPlayerWon = false
            end
        elseif event:type() == 3655313229 then -- EVENT_TYPE_PLAYER_WIN
            if engine.network.isServer then return end

            
        end
        return false;
    end
}