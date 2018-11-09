#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#ifndef GAMEENGINE301CR_MANAGERS_H
#define GAMEENGINE301CR_MANAGERS_H

#include "physics/PhysicsManager.h"
#include "network/NetworkManager.h"
#include "render/RenderManager.h"
#include "entity/EntityManager.h"
#include "SoundManager.h"
#include "AssetManager.h"

static RenderManager gRenderManager;
static EntityManager gEntityManager;
static NetworkManager gNetworkManager;
static PhysicsManager gPhysicsManager;
static SoundManager gSoundManager;
static AssetManager gAssetManager;

#endif //GAMEENGINE301CR_MANAGERS_H

#pragma clang diagnostic pop