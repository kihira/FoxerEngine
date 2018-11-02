#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#ifndef GAMEENGINE301CR_MANAGERS_H
#define GAMEENGINE301CR_MANAGERS_H

#include "AssetManager.h"
#include "network/NetworkManager.h"
#include "render/RenderManager.h"
#include "entity/EntityManager.h"

namespace {
    AssetManager gAssetManager;
    RenderManager gRenderManager;
    EntityManager gEntityManager;
    NetworkManager gNetworkManager;
}

#endif //GAMEENGINE301CR_MANAGERS_H

#pragma clang diagnostic pop