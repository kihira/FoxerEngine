level = {
    id = 248972157, --- LEVEL_1
    name = "Level 1",
    players = 2,
    entities = {
        {
            prototypeId = 4045036281,
            entityId = 12345,
            position = engine.math.vec3.new(1, 0, 1),
            rotation = engine.math.vec3.new(0, 0, 0)
        }
    },
    update = function(self) end,
    events = {
        "EVENT_TYPE_LEVEL_LOAD",
        "EVENT_TYPE_PLAYER_CONNECTED",
        "EVENT_TYPE_PLAYER_DISCONNECTED"
    },
    onEvent = function(self, event)
        if (event:type() == 1994444546) then -- EVENT_TYPE_PLAYER_CONNECTED
            print("Player connected!")
            print("Client ID: " .. event:getUShort("clientId"))

            -- Spawn player entity and assign it
            local player = engine.entity.spawnEntity("ENTITY_PLAYER")
            local clientId = event:getClientId("clientId")
            player.controllingClient = clientId
            self.players[clientId] = player

            -- Send event out so client know its can control it
            local assignEvent = engine.event.event.new("EVENT_TYPE_ASSIGN_PLAYER")
            assignEvent:setClientId("clientId", clientId)
            assignEvent:setEntityId("entityId", player:id())
            assignEvent:push()

        elseif event:type() == 415743068 then -- EVENT_TYPE_PLAYER_DISCONNECTED
            local clientId = event:getClientId("clientId")
            print("Client " .. clientId .. " disconnected")

            -- Ask engine to destroy and remove our reference
            -- engine.entity.destroy(self.players[clientId]:id())
            self.players[clientId] = nil

        elseif event:type() == 1205121214 then -- EVENT_TYPE_LEVEL_LOAD
            print("Level loaded!")

            engine.graphics.camera.position = engine.math.vec3.new(10, 10, 10)
            engine.graphics.camera.target = engine.math.vec3.new(0)

            -- Setup data stuff
            self.players = {}
        end
        return false;
    end
}