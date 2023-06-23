#include <Engine.h>

class Engine : public EngineBase {
public:
    void PostInit() {
        static ImGuiIO io;

        ImGui::CreateContext();
        io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        //ImGui_ImplWin32_InitForOpenGL();
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    void PreRender(std::unique_ptr<Shader>& shader) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /*for (std::shared_ptr<Object*>& obj : level->objects) {
            Object* obj_ptr = *obj;
            if (obj_ptr->type == MESH) {
                Mesh* mesh = dynamic_cast<Mesh*>(obj_ptr);

                mesh->Render(shader);
            }
        }*/
    }
    void PostRender(std::unique_ptr<Shader>& shader) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static glm::vec3 color = glm::vec3(1.0);

        ImGui::Begin("Hello, world!");
        ImGui::SliderFloat3("Color", (float*)&color, 0., 1.);
        ImGui::End();

        shader->Activate();
        shader->setVec3("color", color);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void InputProcess(GLFWwindow* window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    Engine() {
        PostInit();
    }
};

int main() {

    std::unique_ptr<Engine> engine = std::make_unique<Engine>();
    static std::unique_ptr<Shader> base_shader = std::make_unique<Shader>("Base");
    std::shared_ptr<Texture> diffuse_tex1 = std::make_shared<Texture>("WhiteBrick.jpg", DIFFUSE);

    std::vector<float> vertices = {
         // Pos              // Tex Coord
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f // top left 
    };
    std::vector<unsigned int> indices = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
    };
    std::shared_ptr<Mesh> mesh1 = std::make_shared<Mesh>(std::string("Cube"), vertices, indices, ELEMENT_CUSTOM);
    mesh1->SetTextures({diffuse_tex1});
    engine->Add_Object(std::static_pointer_cast<Object>(mesh1));

    glViewport(0, 0, 800, 600);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // WireFrame
    while (!glfwWindowShouldClose(engine->window))
    {
        engine->InputProcess(engine->window);

        engine->PreRender(base_shader);
        engine->PostRender(base_shader);
        
        mesh1->Render(base_shader);

        glfwSwapBuffers(engine->window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;

}