#ifndef FOXERENGINE_SETTINGS_H
#define FOXERENGINE_SETTINGS_H

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

#endif //FOXERENGINE_SETTINGS_H
