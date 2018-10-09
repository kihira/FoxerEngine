#ifndef GAMEENGINE301CR_ASSETMANAGER_H
#define GAMEENGINE301CR_ASSETMANAGER_H


#include <string>
#include "Mesh.h"

class AssetManager {
public:
    Mesh *LoadMesh(std::string name);
};


#endif //GAMEENGINE301CR_ASSETMANAGER_H
