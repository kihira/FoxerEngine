dummyTarget = {
    name = "Dummy",
    collider = {
        gravityScale = 0.3
    },
    render = {
        mesh = "doesnotexist"
    },
    update = function(self)
        -- print("Hello, my name is " .. self.name)
    end
}
