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
            debug_shader->Activate();
            debug_shader->setMat4("model", glm::mat4(1.f));
            debug_shader->setMat4("view", level->main_cam->view);
            debug_shader->setMat4("projection", level->main_cam->proj);
            mesh->aabb_box->Render();
        }
        else if (obj->type == RENDER_OBJECT) {
            std::shared_ptr<RenderObject> r_obj = std::dynamic_pointer_cast<RenderObject>(obj);
            if (r_obj->help_type == LINE) {
                std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(r_obj);

                debug_shader->Activate();
                debug_shader->setMat4("model", glm::mat4(1.0f));
                debug_shader->setMat4("view", level->main_cam->view);
                debug_shader->setMat4("projection", level->main_cam->proj);
                line->Render();
            }
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

static bool Clicked = false;
void EngineBase::InputProcess() {
    if (glfwGetKey(window.GLFW_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window.GLFW_window, true);
    
    if (glfwGetMouseButton(window.GLFW_window, 0) == GLFW_PRESS && !Clicked) {
        Clicked = true;
        double mouseX;
        double mouseY;
        glfwGetCursorPos(window.GLFW_window, &mouseX, &mouseY);
        glm::vec3 ray_dir = Ray::GetScreenToWorld(glm::vec2(mouseX, mouseY), glm::vec2(window.width, window.height), level->main_cam->proj, level->main_cam->view, level->main_cam->transform.position);
        std::shared_ptr<Ray> ray = std::make_shared<Ray>(level->main_cam->transform.position, ray_dir);
        std::shared_ptr<Mesh> obj1 = std::dynamic_pointer_cast<Mesh>(level->objects[0]);
        
        float t = 0;
        std::cout << "AABB - min: X: " << obj1->aabb_box->box.min.x << " Y: " << obj1->aabb_box->box.min.y << " Z: " << obj1->aabb_box->box.min.z << " max: X: " << obj1->aabb_box->box.max.x << " Y: " << obj1->aabb_box->box.max.y << " Z: " << obj1->aabb_box->box.max.z << std::endl;
        std::cout << (t = ray->RayIntersectsAABB(obj1->aabb_box->box, obj1->transform.model)) << std::endl;
        std::cout << "Ray - X: " << ray_dir.x << " Y: " << ray_dir.y << " Z: " << ray_dir.z << std::endl;
        std::cout << "CamRay - X: " << level->main_cam->transform.forward.x << " Y: " << level->main_cam->transform.forward.y << " Z: " << level->main_cam->transform.forward.z << std::endl;
    
        if (t > 0) {
            std::shared_ptr<Line> ray_line = std::make_shared<Line>(ray->origin, ray_dir * 100.f);
            level->Add_Object(std::static_pointer_cast<Object>(ray_line));
        }
    }
    else if (glfwGetMouseButton(window.GLFW_window, 0) == GLFW_RELEASE) {
        Clicked = false;
    }
    if (glfwGetKey(window.GLFW_window, GLFW_KEY_Q) == GLFW_PRESS) {
        std::vector<std::shared_ptr<Object>> objsToErase = {};
        for (std::shared_ptr<Object> obj : level->objects) {
            if (obj->type == RENDER_OBJECT) {
                std::shared_ptr<RenderObject> r_obj = std::dynamic_pointer_cast<RenderObject>(obj);
                if (r_obj->help_type == LINE) {
                    objsToErase.push_back(obj);
                }
            }
        }

        for (std::shared_ptr<Object> obj : objsToErase)
            level->objects.erase(std::remove(level->objects.begin(), level->objects.end(), obj), level->objects.end());
    }
}

EngineBase::EngineBase() {
    Init();
    PostInit();
}

int main() {
    std::unique_ptr<EngineBase> engine = std::make_unique<EngineBase>();
    std::shared_ptr<Shader> base_shader = std::make_shared<Shader>("Base");
    std::shared_ptr<Shader> debug_shader = std::make_shared<Shader>("Debug");
    engine->debug_shader = debug_shader;
    std::shared_ptr<Texture> diffuse_tex1 = std::make_shared<Texture>("WhiteBrick.jpg", DIFFUSE);
    std::shared_ptr<Texture> diffuse_box = std::make_shared<Texture>("container2.png", DIFFUSE);
    std::shared_ptr<Texture> specular_box = std::make_shared<Texture>("container2_specular.png", SPECULAR);
    std::shared_ptr<Texture> emission_box = std::make_shared<Texture>("matrix.jpg", EMISSION);

    std::shared_ptr<SMaterial> mainColorMaterial = std::make_shared<SMaterial>();
    mainColorMaterial->Diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
    mainColorMaterial->Ambient = glm::vec3(1.0f, 0.5f, 0.31f);
    mainColorMaterial->Specular = glm::vec3(1.0f);
    mainColorMaterial->diffuse_texture = diffuse_box;
    mainColorMaterial->specular_texture = specular_box;
    mainColorMaterial->emission_texture = emission_box;
    std::shared_ptr<SMaterial> lightMaterial = std::make_shared<SMaterial>();
    lightMaterial->Diffuse = glm::vec3(1.0f);
    lightMaterial->Ambient = glm::vec3(1.0f);
    lightMaterial->Emission = glm::vec3(1.0f);
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
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(1.0f,  3.0f, -5.0f),
    };
     
    for (int i = 0; i < 2; i++) {
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(std::string("Cube"), vertices);
        mesh->transform.Translate(cubePositions[i]);
        mesh->transform.Rotate(glm::vec3(45.f, 0.f, 0.f));
        mesh->aabb_box->CalculateMinMax(mesh->transform.model, true);
        mesh->SetShader(base_shader);
        mesh->SetMaterial(mainColorMaterial);
        engine->Add_Object(std::static_pointer_cast<Object>(mesh));
        std::cout << "X: " << mesh->transform.forward.x << " Y: " << mesh->transform.forward.y << " Z: " << mesh->transform.forward.z << std::endl;
    }

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(std::string("Cube"), vertices);
    mesh->transform.Scale(glm::vec3(0.1f));
    mesh->transform.Translate(glm::vec3(-2, 1, 0));
    mesh->SetShader(base_shader);
    mesh->SetMaterial(lightMaterial);
    engine->Add_Object(std::static_pointer_cast<Object>(mesh));
    
    
    mesh->aabb_box->CalculateMinMax(mesh->transform.model, true);
    light->transform.Translate(glm::vec3(-2, 1, 0));


    std::shared_ptr<Camera> cam1 = std::make_shared<Camera>(engine->window.width, engine->window.height, 60.f, PERSPECTIVE);
    cam1->transform.Translate(glm::vec3(1,1,-1));
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

        base_shader->setVec3("light.position", light->transform.position);
        base_shader->setVec3("light.AmbientStrength", glm::vec3(0.2f));
        base_shader->setVec3("light.DiffuseStrength", glm::vec3(0.5f));
        base_shader->setVec3("light.SpecularStrength", glm::vec3(1.0f));
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