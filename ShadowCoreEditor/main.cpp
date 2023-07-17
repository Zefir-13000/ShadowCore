#include <Core/Core.h>

using namespace SC;

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
            /*debug_shader->Activate();
            debug_shader->setVec3("color", glm::vec3(0.8f));
            debug_shader->setMat4("model", glm::mat4(1.f));
            debug_shader->setMat4("view", level->main_cam->view);
            debug_shader->setMat4("projection", level->main_cam->proj);

            //mesh->aabb_box->Render();*/
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
    
    if (glfwGetMouseButton(window.GLFW_window, 0) == GLFW_PRESS) {
        Clicked = true;
        double mouseX;
        double mouseY;
        glfwGetCursorPos(window.GLFW_window, &mouseX, &mouseY);
        glm::vec3 ray_dir = Ray::GetScreenToWorld(glm::vec2(mouseX, mouseY), glm::vec2(window.width, window.height), level->main_cam->proj, level->main_cam->view, level->main_cam->transform.position);
        std::shared_ptr<Ray> ray = std::make_shared<Ray>(level->main_cam->transform.position, ray_dir);
        std::shared_ptr<Mesh> obj1 = std::dynamic_pointer_cast<Mesh>(level->objects[0]);
        
        float t = 0;
        std::shared_ptr<AABB> aabb = obj1->GetComponent<AABB>();
        if (aabb != nullptr) {
            //std::cout << "AABB - min: X: " << obj1->aabb_box->box.min.x << " Y: " << obj1->aabb_box->box.min.y << " Z: " << obj1->aabb_box->box.min.z << " max: X: " << obj1->aabb_box->box.max.x << " Y: " << obj1->aabb_box->box.max.y << " Z: " << obj1->aabb_box->box.max.z << std::endl;
            std::cout << (t = ray->RayIntersectsAABB(aabb->box, obj1->transform.model)) << std::endl;
            //std::cout << "Ray - X: " << ray_dir.x << " Y: " << ray_dir.y << " Z: " << ray_dir.z << std::endl;
            //std::cout << "CamRay - X: " << level->main_cam->transform.forward.x << " Y: " << level->main_cam->transform.forward.y << " Z: " << level->main_cam->transform.forward.z << std::endl;
        }
        if (t > 0) {
            glm::vec3 hitPoint = ray->origin + ray_dir * t;
            std::shared_ptr<Point> ray_point = std::make_shared<Point>(hitPoint, 8.f);

            std::shared_ptr<Line> ray_line = std::make_shared<Line>(ray->origin, ray->origin + ray_dir * 100.f);
            level->Add_Object(std::static_pointer_cast<Object>(ray_line));
            level->Add_Object(std::static_pointer_cast<Object>(ray_point));

            //obj1->RemoveComponent(aabb);

        }
        else {
            std::shared_ptr<Line> ray_line = std::make_shared<Line>(ray->origin, ray->origin + ray_dir * 100.f);
            level->Add_Object(std::static_pointer_cast<Object>(ray_line));
        }
    }
    else if (glfwGetMouseButton(window.GLFW_window, 0) == GLFW_RELEASE) {
        Clicked = false;
    }
    if (glfwGetKey(window.GLFW_window, GLFW_KEY_Q) == GLFW_PRESS) {
        std::vector<std::shared_ptr<Object>> objsToErase = {};
        for (std::shared_ptr<Object> obj : level->objects) {
            if (obj->type == MESH) {
                std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(obj);
                if (mesh->meshType == LINE_TYPE || mesh->meshType == POINT_TYPE) {
                    objsToErase.push_back(obj);
                }
            }
        }

        for (std::shared_ptr<Object> obj : objsToErase)
            level->Destroy_Object(obj);
    }
}

EngineBase::EngineBase() {
    Init();
    Init_Shaders();
    PostInit();
}

int main() {
    Core::Init();
    std::shared_ptr<EngineBase> enginePtr = Core::Engine;
    std::shared_ptr<Level> level = enginePtr->level;

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

    std::shared_ptr<std::vector<float>> vertices = std::make_shared<std::vector<float>>(std::initializer_list<float>{
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
    });

    glm::vec3 cubePositions[] = {
        glm::vec3(1.0f,  0.0f,  0.0f),
        glm::vec3(1.0f,  3.0f, -5.0f),
    };
     
    for (int i = 0; i < 2; i++) {
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(std::string("Cube"), vertices);
        mesh->transform.Translate(cubePositions[i]);
        mesh->transform.Rotate(glm::vec3(45.f, 0.f, 0.f));
        //mesh->aabb_box->CalculateMinMax(mesh->transform.model, true);
        std::shared_ptr<AABB> aabb = std::make_shared<AABB>(mesh->vertices);
        aabb->CalculateMinMax(mesh->transform.model, true);
        mesh->AddComponent(aabb);
        mesh->SetShader(enginePtr->standart_render_shader);
        mesh->SetMaterial(mainColorMaterial);
        level->Add_Object(std::static_pointer_cast<Object>(mesh));
        std::cout << "X: " << mesh->transform.forward.x << " Y: " << mesh->transform.forward.y << " Z: " << mesh->transform.forward.z << std::endl;
    }

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(std::string("Cube"), vertices);
    mesh->transform.Scale(glm::vec3(0.1f));
    mesh->transform.Translate(glm::vec3(-2, 1, 0));
    std::shared_ptr<AABB> aabb = std::make_shared<AABB>(mesh->vertices);
    aabb->CalculateMinMax(mesh->transform.model, true);
    mesh->AddComponent(aabb);
    //mesh->aabb_box->CalculateMinMax(mesh->transform.model, true);
    mesh->SetShader(enginePtr->standart_render_shader);
    mesh->SetMaterial(lightMaterial);
    level->Add_Object(std::static_pointer_cast<Object>(mesh));

    std::shared_ptr<Cone> cone = std::make_shared<Cone>(0.3f, 0.1f);
    cone->transform.Translate(glm::vec3(2, 1, 0));
    //cone->aabb_box->CalculateMinMax(cone->transform.model, true);
    cone->SetShader(enginePtr->standart_render_shader);
    level->Add_Object(std::static_pointer_cast<Object>(cone));
    
    
    light->transform.Translate(glm::vec3(-2, 1, 0));


    std::shared_ptr<Camera> cam1 = std::make_shared<Camera>(enginePtr->window.width, enginePtr->window.height, 60.f, PERSPECTIVE);
    cam1->transform.Translate(glm::vec3(1,1,-1));
    level->main_cam = cam1;


    glViewport(0, 0, enginePtr->window.width, enginePtr->window.height);
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // WireFrame
    while (!glfwWindowShouldClose(enginePtr->window.GLFW_window))
    {
        enginePtr->InputProcess();

        enginePtr->standart_render_shader->Activate();
        enginePtr->standart_render_shader->setMat4("view", cam1->view);
        enginePtr->standart_render_shader->setMat4("projection", cam1->proj);

        enginePtr->standart_render_shader->setVec3("light.position", light->transform.position);
        enginePtr->standart_render_shader->setVec3("light.AmbientStrength", glm::vec3(0.2f));
        enginePtr->standart_render_shader->setVec3("light.DiffuseStrength", glm::vec3(0.5f));
        enginePtr->standart_render_shader->setVec3("light.SpecularStrength", glm::vec3(1.0f));
        enginePtr->standart_render_shader->setVec3("light.color", light->light_color);

        enginePtr->standart_render_shader->setVec3("viewPos", cam1->transform.position);

        enginePtr->PreRender();
        enginePtr->PostRender();

        cam1->Movement(enginePtr->window.GLFW_window, enginePtr->window.width, enginePtr->window.height);

        glfwSwapBuffers(enginePtr->window.GLFW_window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;

}