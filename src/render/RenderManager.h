
#ifndef FOXERENGINE_RENDERMANAGER_H
#define FOXERENGINE_RENDERMANAGER_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

class Light;
class RenderComponent;
class Shader;
class Camera;
class WindowWrapper;

#define GLERRCHECK() RenderManager::GLERRCHECK_fn(__FILE__, __LINE__)

class RenderManager {
private:
    std::unique_ptr<WindowWrapper> windowWrapper;
    std::unique_ptr<Camera> camera;
    GLuint currentShader = 0;
    std::shared_ptr<spdlog::logger> logger;
    std::vector<RenderComponent *> renderComponents;
	std::vector<Light *> lights;

    static void glfwErrorCallback(int error, const char *desc);

    static void glfwFramebufferSizeCallback(int width, int height);
public:
    RenderManager();

    ~RenderManager();

    /**
     * Initialises the RenderManager
     */
    void startUp();

    /**
     * De-initialises the RenderManager
     */
    void shutDown();

    void frameStart();

    void frameEnd();

    void update(float deltaTime);

    /**
     * Checks whether the GLFW window is attempting to close
     * @return
     */
    int shouldClose();

    /**
     * Begins using the shader and sets up the view and projection matrices
     * @param shader
     */
    void useShader(std::shared_ptr<Shader> shader);

    void addRenderComponent(RenderComponent *component);

	void removeRenderComponent(RenderComponent *component);

    WindowWrapper *getWindowWrapper() const;

    const Camera *getCamera() const;

	/**
	 * Adds a light that will be passed into shaders
	 */
	void addLight(Light *light);

	/**
	 * Removes a light from the render pipeline
	 */
	void removeLight(Light *light);

    static void GLERRCHECK_fn(const char *file, int line);
};


#endif //FOXERENGINE_RENDERMANAGER_H
