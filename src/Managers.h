#ifndef GAMEENGINE301CR_MANAGERS_H
#define GAMEENGINE301CR_MANAGERS_H
#pragma once

#include "AssetManager.h"
#include "physics/PhysicsManager.h"
#include "network/NetworkManager.h"
#include "render/RenderManager.h"
#include "entity/EntityManager.h"
#include "SoundManager.h"

// Gotta use extern here so we can access the same instance across all files
// It totally didn't take me 2 days to "fix" it from being static and going crazy
extern RenderManager gRenderManager;
extern EntityManager gEntityManager;
extern NetworkManager gNetworkManager;
extern PhysicsManager gPhysicsManager;
extern SoundManager gSoundManager;
extern AssetManager gAssetManager;

#endif //GAMEENGINE301CR_MANAGERS_H