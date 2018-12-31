level = {
    name = "Level 1",
    players = 2,
    update = function(self) end,
    events = {
        "EVENT_TYPE_PLAYER_CONNECTED" --- 1994444546
    },
    onEvent = function(self, event)
        if (event.type == 1994444546) then
            print("Player connected!")
        end
    end
}