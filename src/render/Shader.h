#ifndef GAMEENGINE301CR_SHADER_H
#define GAMEENGINE301CR_SHADER_H


#include <map>

class Shader final {
private:
    unsigned int program = 0;
    std::map<const char *, int> uniforms;
public:
    explicit Shader(unsigned int program);

    ~Shader();

    /**
     * Pre-emptively looks up a uniforms location and stores it for faster access later
     * @param name
     */
    void registerUniform(const char *name);
    /**
     *
     * @tparam T
     * @param name
     * @param value
     */
    template <typename T>
    void setUniform(const char *name, T value);

    unsigned int getProgram() const;
};


#endif //GAMEENGINE301CR_SHADER_H
