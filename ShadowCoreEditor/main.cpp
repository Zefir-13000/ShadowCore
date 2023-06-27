#include <Engine.h>

void EngineBase::PostInit() {
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
    ImGui_ImplGlfw_InitForOpenGL(window.GLFW_window, true);
    //ImGui_ImplWin32_InitForOpenGL();
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void EngineBase::PreRender() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (std::shared_ptr<Object> obj : level->objects) {
        if (obj->type == MESH) {
            std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(obj);
            mesh->Render();
        }
    }
    EngineBase::EngineLoop();
}
void EngineBase::PostRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    

    ImGui::Begin("Hello, world!");
    //ImGui::SliderFloat3("Color", (float*)&color, 0., 1.);
    ImGui::End();

    //shader->Activate();
    //shader->setVec3("color", color);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EngineBase::InputProcess() {
    if (glfwGetKey(window.GLFW_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window.GLFW_window, true);
}

EngineBase::EngineBase() {
    Init();
    PostInit();
}

int main() {
    std::unique_ptr<EngineBase> engine = std::make_unique<EngineBase>();
    std::shared_ptr<Shader> base_shader = std::make_shared<Shader>("Base");
    std::shared_ptr<Texture> diffuse_tex1 = std::make_shared<Texture>("WhiteBrick.jpg", DIFFUSE);

    std::shared_ptr<MaterialTexture> mainTexMaterial = std::make_shared<MaterialTexture>(diffuse_tex1);
    std::shared_ptr<MaterialColor> mainColorMaterial = std::make_shared<MaterialColor>();
    mainColorMaterial->Albedo = glm::vec3(0.5, 0.38, 0.68);
    mainColorMaterial->Ambient = 0.3f;
    std::shared_ptr<MaterialColor> lightMaterial = std::make_shared<MaterialColor>();
    lightMaterial->Albedo = glm::vec3(1.0);
    lightMaterial->Ambient = 1.0f;
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(glm::vec3(1.0f), 1.0f);

    std::vector<float> vertices = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(1.0f,  -1.0f,  0.0f),
        glm::vec3(1.0f,  3.0f, -5.0f),
    };
     
    for (int i = 0; i < 2; i++) {
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(std::string("Cube"), vertices, ARRAY_CUSTOM);
        mesh->transform.Translate(cubePositions[i]);
        mesh->transform.Rotate(cubePositions[i]);
        mesh->SetShader(base_shader);
        mesh->SetMaterial(mainColorMaterial);
        engine->Add_Object(std::static_pointer_cast<Object>(mesh));
        std::cout << "X: " << mesh->transform.forward.x << " Y: " << mesh->transform.forward.y << " Z: " << mesh->transform.forward.z << std::endl;
    }

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(std::string("Cube"), vertices, ARRAY_CUSTOM);
    mesh->transform.Scale(glm::vec3(0.1f));
    mesh->SetShader(base_shader);
    mesh->SetMaterial(lightMaterial);
    engine->Add_Object(std::static_pointer_cast<Object>(mesh));
    

    std::shared_ptr<Camera> cam1 = std::make_shared<Camera>(engine->window.width, engine->window.height, 60.f, PERSPECTIVE);
    engine->level->main_cam = cam1;

    glViewport(0, 0, engine->window.width, engine->window.height);
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // WireFrame
    while (!glfwWindowShouldClose(engine->window.GLFW_window))
    {
        engine->InputProcess();

        base_shader->Activate();
        base_shader->setMat4("view", cam1->view);
        base_shader->setMat4("projection", cam1->proj);

        base_shader->setVec3("light.postion", light->transform.position);
        base_shader->setVec3("light.color", light->light_color);
        base_shader->setVec3("viewPos", cam1->transform.position);

        engine->PreRender();
        engine->PostRender();

        cam1->Movement(engine->window.GLFW_window, engine->window.width, engine->window.height);

        glfwSwapBuffers(engine->window.GLFW_window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;

}