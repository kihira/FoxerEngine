
#ifndef GAMEENGINE301CR_OPENGL_HELPER_H
#define GAMEENGINE301CR_OPENGL_HELPER_H

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <spdlog/spdlog.h>

#define GLERRCHECK() GLERRCHECK_fn(__FILE__, __LINE__)

// todo move this into render manager
namespace {
    void GLERRCHECK_fn(const char *file, int line) {
        GLenum err = glGetError();
        if (err == GL_NO_ERROR) return;

        std::string error;

        switch (err) {
            case GL_INVALID_ENUM:
                error = "INVALID ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT OF MEMORY";
                break;
            default:
                error = "UNKNOWN ERROR";
                break;
        }

        spdlog::get("renderer")->error("({}:{}) GL ERROR 0x{:x}: {}", file, line, err, error);
    }
}

#endif //GAMEENGINE301CR_OPENGL_HELPER_H
