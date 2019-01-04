#ifndef GAMEENGINE301CR_SETTINGS_H
#define GAMEENGINE301CR_SETTINGS_H

#include <string>
#include "util/StringId.h"

struct Settings {
    std::string windowTitle = "Default Window";
    int windowWidth = 1920;
    int windowHeight = 1080;

    float cameraFov = 75.f;

    StringId initialLevel = 0;

    float tickRate = 1.f / 60.f;
};

#endif //GAMEENGINE301CR_SETTINGS_H
