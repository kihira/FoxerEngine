return {
    name = "Portal",
    position = engine.math.vec3.new(0.0),
    renderComponent = {
        mesh = 2795621949, -- MESH_PORTAL
		shader = 2946951896
    },
    physicsComponent = {
        type = 1,
        shape = 1,
        halfWidth = 2,
        halfHeight = 2,
        centreX = -5,
        centreZ = 2,
        beginContact = function(self, other)
            if (engine.network.isClient()) then
                engine.sound.play(3989746563)
            end
        end,
        endContact = function(self, other)
        end
    }
}
