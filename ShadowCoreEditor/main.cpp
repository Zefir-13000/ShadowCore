#include "Editor.h"

using namespace SC;

int main() {
    Core::Init();

    std::shared_ptr<Engine> enginePtr = Core::Engine;
    std::shared_ptr<Level> level = enginePtr->level;

    std::shared_ptr<FlyCamera> cam1 = std::make_shared<FlyCamera>(enginePtr->window.width, enginePtr->window.height, 60.f, PERSPECTIVE);

    cam1->transform->Translate(glm::vec3(1, 1, -1));
    level->main_cam = cam1;

    std::shared_ptr<Texture> diffuse_tex1 = std::make_shared<Texture>("WhiteBrick.jpg", DIFFUSE_TEXTURE);
    std::shared_ptr<Texture> diffuse_box = std::make_shared<Texture>("container2.png", DIFFUSE_TEXTURE);
    std::shared_ptr<Texture> specular_box = std::make_shared<Texture>("container2_specular.png", SPECULAR_TEXTURE);
    std::shared_ptr<Texture> emission_box = std::make_shared<Texture>("matrix.jpg", EMISSION_TEXTURE);

    std::shared_ptr<SMaterial> mainColorMaterial = std::make_shared<SMaterial>();
    mainColorMaterial->Ambient = glm::vec3(1.0f, 0.5f, 0.31f);
    mainColorMaterial->diffuse_texture = diffuse_box;
    mainColorMaterial->specular_texture = specular_box;
    mainColorMaterial->emission_texture = emission_box;
    std::shared_ptr<SMaterial> whiteColorMaterial = std::make_shared<SMaterial>();
    whiteColorMaterial->Diffuse = glm::vec3(1.0f);
    mainColorMaterial->Ambient = glm::vec3(0.3f);
    mainColorMaterial->Specular = glm::vec3(1.0f);
    std::shared_ptr<SMaterial> lightMaterial = std::make_shared<SMaterial>();
    lightMaterial->Diffuse = glm::vec3(1.0f);
    lightMaterial->Ambient = glm::vec3(1.0f);
    lightMaterial->Emission = glm::vec3(1.0f);

    std::shared_ptr<DirectionalLight> dir_light = std::make_shared<DirectionalLight>(glm::normalize(glm::vec3(0.f, 1.f, 0.f)), 1.0f);
    dir_light->InitShadows();
    level->Add_Object(dir_light);

    std::shared_ptr<Model> dragon = std::make_shared<Model>("Models/dragon.obj", MANDATORY);
    dragon->transform->Scale(glm::vec3(0.5f));
    dragon->transform->Translate(glm::vec3(0, 0.136f, 0));
    dragon->transform->Update();
    dragon->AddComponent<AABB>();
    level->Add_Object(dragon);


    /*std::shared_ptr<Model> sponza = std::make_shared<Model>("Models/sponza.obj", MANDATORY);
    sponza->transform->Scale(glm::vec3(0.002f));
    sponza->transform->Translate(glm::vec3(0, 0, 0));
    sponza->transform->Update();
    sponza->AddComponent<AABB>();
    level->Add_Object(sponza);*/
    
    std::shared_ptr<Cube> mesh = std::make_shared<Cube>();
    mesh->transform->Scale(glm::vec3(0.1f));
    mesh->transform->Translate(glm::vec3(-0.609f, 2.f, 0.f));
    mesh->AddComponent<AABB>();
    mesh->SetMaterial(lightMaterial);
    level->Add_Object(mesh);

    std::shared_ptr<Plane> plane = std::make_shared<Plane>(3, 3);
    plane->SetMaterial(whiteColorMaterial);
    level->Add_Object(plane);

    std::shared_ptr<Cone> cone = std::make_shared<Cone>(0.3f, 0.1f);
    cone->transform->Translate(glm::vec3(2, 1, 0));
    cone->AddComponent<AABB>();
    cone->SetMaterial(lightMaterial);
    level->Add_Object(cone);

    glViewport(0, 0, enginePtr->window.width, enginePtr->window.height);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glCullFace(GL_BACK);

    Editor::EditorObjectsInit();
    while (!glfwWindowShouldClose(enginePtr->window.GLFW_window))
    {
        enginePtr->InputProcess();

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