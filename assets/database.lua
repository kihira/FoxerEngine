database = {
    levels = {
        [248972157] = "level.lua"
    },
    entities = {
        [591432690] = "player.lua", -- ENTITY_PLAYER
        [315271780] = "ground.lua", -- ENTITY_GROUND
		[1675181212] = "groundRough.lua", -- ENTITY_GROUND_ROUGH
        [592045845] = "portal.lua", -- ENTITY_PORTAL,
        [2979648629] = "waitingPlayers.lua", -- ENTITY_WAITING_PLAYERS
        [491081606] = "player1Win.lua", -- ENTITY_PLAYER_ONE_WIN
        [2872810798] = "player2Win.lua", -- ENTITY_PLAYER_TWO_WIN
        [1316782920] = "youLose.lua",
        [2252448813] = "youWin.lua"
    },
    meshes = {
        [3415080175] = "spaceCraft1.obj", -- MESH_SPACECRAFT
        [3061288721] = "floor.obj", -- MESH_FLOOR
		[2381128972] = "groundTile.obj", -- MESH_GROUND
        [4214878900] = "groundTileRough.obj", -- MESH_GROUND_ROUGH
        [2795621949] = "portal.obj", -- MESH_PORTAL
        [2992890141] = "waiting-players.obj", -- MESH_WAITING_PLAYERS
        [1317505198] = "player1Win.obj", -- MESH_PLAYER_ONE_WIN
        [1730504262] = "player2Win.obj", -- MESH_PLAYER_TWO_WIN
        [1982698336] = "youLose.obj",
        [3313820357] = "youWin.obj"
    },
    textures = {
        [2300579869] = {
            file = "floor.png"
        }
    },
    shaders = {
        [2946951896] = { -- SHADER_DEFAULT
            fragFile = "simple_frag.glsl",
            vertFile = "simple_vert.glsl",
			uniforms = {
				"material.ambient",
				"material.diffuse",
				"material.specular"
			}
        }
    },
	sounds = {
		[2016780361] = { -- SOUND_BACKGROUND_MUSIC
			file = "background.ogg"
        },
        [3989746563] = { -- SOUND_POWERUP
            file = "powerup.wav"
        }
	},
    events = {
        [1994444546] = { -- EVENT_TYPE_PLAYER_CONNECTED
            args = {
                "clientId"
            }
        },
        [415743068] = { -- EVENT_TYPE_PLAYER_DISCONNECTED
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
        },
        [2577080706] = { -- MESSAGE_PHYSICS_SYNC
            args = {
                "entityId",
                "posX",
                "posY",
                "posZ",
                "rotX",
                "rotY",
                "rotZ",
                "velX",
                "velZ",
                "velA"
            }
        },
        [3819351460] = { -- EVENT_TYPE_LEVEL_LOAD_SERVER
            args = {
                "levelId"
            }
        },
        [3302156521] = { -- EVENT_TYPE_PLAY_SOUND
            args = {
                "soundId"
            }
        },
        [3655313229] = { -- EVENT_TYPE_PLAYER_WIN
            args = {
                "entityId"
            }
        }
    }
}