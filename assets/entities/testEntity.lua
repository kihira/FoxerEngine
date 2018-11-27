testEntity = {
    name = "Test Entity",
    collider = {
        gravityScale = 0.3
    },
    render = {
        mesh = "testMesh"
    },
    update = function(self)
        -- print("Hello, my name is " .. self.name)
    end,
    onSpawn = function(self)
        print("Oh no, I've spawned!")
    end
}
