database = {
    levels = {
        [248972157] = "level.lua"
    },
    entities = {
        [591432690] = "player.lua"
    },
    meshes = {
        [3061288721] = "floor.obj"
    },
    textures = {
        [2300579869] = {
            file = "floor.png",

        }
    },
    shaders = {
        [2946951896] = {
            name = "SHADER_DEFAULT",
            fragFile = "simple_frag.glsl",
            vertFile = "simple_vert.glsl"
        }
    },
    events = {
        [1994444546] = {
            name = "EVENT_TYPE_PLAYER_CONNECTED",
            args = {
                ["clientId"] = {
                    type = "TYPE_USHORT"
                }
            }
        },
        [1770460267] = {
            name = "EVENT_TYPE_INPUT_PLAYER",
            args = {
                ["entityId"] = {
                    type = "TYPE_USHORT"
                },
                ["inputBitmask"] = {
                    type = "TYPE_UINT"
                }
            }
        }
    }
}