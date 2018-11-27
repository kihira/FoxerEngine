#ifndef GAMEENGINE301CR_SETTINGS_H
#define GAMEENGINE301CR_SETTINGS_H

#include <string>

struct Settings {
    std::string windowTitle = "Default Window";
    int windowWidth = 1920;
    int windowHeight = 1080;

    float cameraFov = 75.f;

    std::string initialLevel = "mainmenu";
};

#endif //GAMEENGINE301CR_SETTINGS_H
