#include <Engine.h>

class Engine : public EngineBase {
public:
    void PreRender() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    void PostRender() {

    }

    void InputProcess(GLFWwindow* window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
};

int main() {
    std::unique_ptr<Engine> engine = std::make_unique<Engine>();

    glViewport(0, 0, 800, 600);
    while (!glfwWindowShouldClose(engine->window))
    {
        engine->InputProcess(engine->window);

        engine->PreRender();
        engine->PostRender();

        glfwSwapBuffers(engine->window);
        glfwPollEvents();
    }

    return 0;

}