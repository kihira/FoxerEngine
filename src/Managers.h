#ifndef FOXERENGINE_MANAGERS_H
#define FOXERENGINE_MANAGERS_H
#pragma once

// Forward declare for performance
class RenderManager;
class EntityManager;
class NetworkManager;
class PhysicsManager;
class SoundManager;
class AssetManager;
class InputManager;
class EventManager;
class LevelManager;

// Gotta use extern here so we can access the same instance across all files
// It totally didn't take me 2 days to "fix" it from being static and going crazy
extern RenderManager gRenderManager;
extern EntityManager gEntityManager;
extern NetworkManager gNetworkManager;
extern PhysicsManager gPhysicsManager;
extern SoundManager gSoundManager;
extern AssetManager gAssetManager;
extern InputManager gInputManager;
extern EventManager gEventManager;
extern LevelManager gLevelManager;

#endif //FOXERENGINE_MANAGERS_H