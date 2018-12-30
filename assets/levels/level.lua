level = {
    name = "Level 1",
    players = 2,
    update = function(self) end,
    events = {
        "EVENT_TYPE_PLAYER_CONNECTED"
    },
    onEvent = function(self, event)
        if (event.type == "EVENT_TYPE_PLAYER_CONNECTED") then
            print("Player connected!")
        end
    end
}