#include "Editor.h"

using namespace SC;

int main() {
    Core::Init();

    std::shared_ptr<Engine> enginePtr = Core::Engine;
    std::shared_ptr<Level> level = enginePtr->level;

    std::shared_ptr<FlyCamera> cam1 = std::make_shared<FlyCamera>(enginePtr->window.width, enginePtr->window.height, 60.f, PERSPECTIVE);
    cam1->transform.Translate(glm::vec3(1, 1, -1));
    level->main_cam = cam1;

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
    ImGui::DestroyContext();
    return 0;

}