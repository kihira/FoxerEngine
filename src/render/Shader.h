#ifndef GAMEENGINE301CR_SHADER_H
#define GAMEENGINE301CR_SHADER_H


#include <map>

class Shader {
private:
    GLuint program = 0;
    std::map<std::string, GLint> uniforms;
public:
    Shader(GLuint program);
    void use();
    /**
     * Pre-emptively looks up a uniforms location and stores it for faster access later
     * @param name
     */
    void registerUniform(std::string name);
    /**
     *
     * @tparam T
     * @param name
     * @param value
     */
    template <typename T>
    void setUniform(const std::string &name, T value);
};


#endif //GAMEENGINE301CR_SHADER_H
