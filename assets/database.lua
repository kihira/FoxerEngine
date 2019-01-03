database = {
    levels = {
        [248972157] = "level.lua"
    },
    entities = {
        [591432690] = "player.lua", -- ENTITY_PLAYER
        [4045036281] = "floor.lua" -- ENTITY_FLOOR
    },
    meshes = {
        [3061288721] = "floor.obj" -- MESH_FLOOR
    },
    textures = {
        [2300579869] = {
            file = "floor.png",

        }
    },
    shaders = {
        [2946951896] = { -- SHADER_DEFAULT
            fragFile = "simple_frag.glsl",
            vertFile = "simple_vert.glsl"
        }
    },
    events = {
        [1994444546] = { -- EVENT_TYPE_PLAYER_CONNECTED
            args = {
                "clientId"
            }
        },
        [1770460267] = { -- EVENT_TYPE_INPUT_PLAYER
            args = {
                "entityId",
                "inputBitmask"
            }
        },
        [791803502] = { -- EVENT_TYPE_ENTITY_SPAWN
            args = {
                "entityId",
                "prototypeId"
            }
        },
        [697357692] = { -- EVENT_TYPE_ASSIGN_PLAYER
            args = {
                "entityId",
                "clientId"
            }
        },
        [501658455] = { -- EVENT_TYPE_ENTITY_DESTROY
            args = {
                "entityId"
            }
        }
    }
}