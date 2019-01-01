level = {
    id = 248972157, --- LEVEL_1
    name = "Level 1",
    players = 2,
    update = function(self) end,
    events = {
        "EVENT_TYPE_LEVEL_LOAD",
        "EVENT_TYPE_PLAYER_CONNECTED"
    },
    onEvent = function(self, event)
        if (event:type() == 1994444546) then --- EVENT_TYPE_PLAYER_CONNECTED
            print("Player connected!")
            print("Client ID: " .. event:getUShort("clientId"))
        elseif event:type() == 1205121214 then --- EVENT_TYPE_LEVEL_LOAD
            print("Level loaded!")
            engine.entity.spawnEntity("testEntity")
        end
        return false;
    end
}