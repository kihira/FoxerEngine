#ifndef GAMEENGINE301CR_MANAGERS_H
#define GAMEENGINE301CR_MANAGERS_H
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

#endif //GAMEENGINE301CR_MANAGERS_H