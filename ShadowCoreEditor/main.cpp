#include "Editor.h"

using namespace SC;

int main() {
    Core::Init();

    std::shared_ptr<Engine> enginePtr = Core::Engine;
    std::shared_ptr<Level> level = enginePtr->level;

    std::shared_ptr<FlyCamera> cam1 = std::make_shared<FlyCamera>(enginePtr->window.width, enginePtr->window.height, 60.f, PERSPECTIVE);
    std::shared_ptr<Camera> shadow1_cam = std::make_shared<Camera>(enginePtr->window.width, enginePtr->window.height, 1.f, ORTHOGRAPHIC);
    shadow1_cam->far_plane = 10.f;

    cam1->transform->Translate(glm::vec3(1, 1, -1));
    level->main_cam = cam1;

    std::shared_ptr<Texture> diffuse_tex1 = std::make_shared<Texture>("WhiteBrick.jpg", DIFFUSE_TEXTURE);
    std::shared_ptr<Texture> diffuse_box = std::make_shared<Texture>("container2.png", DIFFUSE_TEXTURE);
    std::shared_ptr<Texture> specular_box = std::make_shared<Texture>("container2_specular.png", SPECULAR_TEXTURE);
    std::shared_ptr<Texture> emission_box = std::make_shared<Texture>("matrix.jpg", EMISSION_TEXTURE);

    std::shared_ptr<SMaterial> mainColorMaterial = std::make_shared<SMaterial>();
    mainColorMaterial->Diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
    mainColorMaterial->Ambient = glm::vec3(1.0f, 0.5f, 0.31f);
    mainColorMaterial->Specular = glm::vec3(1.0f);
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
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(glm::vec3(1.0f), 1.0f);
    light->transform->Translate(glm::vec3(-2, 1, 0));

    std::shared_ptr<Model> dragon = std::make_shared<Model>("Models/dragon.obj", MANDATORY);
    dragon->transform->Scale(glm::vec3(0.5f));
    dragon->transform->Translate(glm::vec3(0, 0.136f, 0));
    dragon->transform->Update();
    dragon->AddComponent<AABB>();
    dragon->SetShader(enginePtr->standart_render_shader);
    level->Add_Object(dragon);


    std::shared_ptr<Model> sponza = std::make_shared<Model>("Models/sponza.obj", MANDATORY);
    sponza->transform->Scale(glm::vec3(0.002f));
    sponza->transform->Translate(glm::vec3(0, 0, 0));
    sponza->transform->Update();
    sponza->AddComponent<AABB>();
    sponza->SetShader(enginePtr->standart_render_shader);
    level->Add_Object(sponza);
    
    std::shared_ptr<Cube> mesh = std::make_shared<Cube>();
    mesh->transform->Scale(glm::vec3(0.1f));
    mesh->transform->Translate(glm::vec3(-0.609f, 2.f, 0.f));
    mesh->AddComponent<AABB>();
    mesh->SetShader(enginePtr->standart_render_shader);
    mesh->SetMaterial(lightMaterial);
    level->Add_Object(mesh);

    std::shared_ptr<Plane> plane = std::make_shared<Plane>(3, 3);
    plane->SetShader(enginePtr->standart_render_shader);
    plane->SetMaterial(whiteColorMaterial);
    level->Add_Object(plane);

    std::shared_ptr<Cone> cone = std::make_shared<Cone>(0.3f, 0.1f);
    cone->transform->Translate(glm::vec3(2, 1, 0));
    cone->AddComponent<AABB>();
    cone->SetMaterial(lightMaterial);
    cone->SetShader(enginePtr->standart_render_shader);
    level->Add_Object(cone);

    light->transform = mesh->transform;
    shadow1_cam->transform = mesh->transform;
    level->Add_Shadow(shadow1_cam, 2048);

    glViewport(0, 0, enginePtr->window.width, enginePtr->window.height);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glCullFace(GL_BACK);

    Editor::EditorObjectsInit();
    while (!glfwWindowShouldClose(enginePtr->window.GLFW_window))
    {
        enginePtr->InputProcess();
        enginePtr->standart_render_shader->Activate();

        enginePtr->standart_render_shader->setValue("light.position", light->transform->position);
        enginePtr->standart_render_shader->setValue("light.AmbientStrength", glm::vec3(0.2f));
        enginePtr->standart_render_shader->setValue("light.DiffuseStrength", glm::vec3(0.5f));
        enginePtr->standart_render_shader->setValue("light.SpecularStrength", glm::vec3(1.0f));
        enginePtr->standart_render_shader->setValue("light.color", light->light_color);

        enginePtr->standart_render_shader->setValue("viewPos", cam1->transform->position);
        enginePtr->standart_render_shader->setValue("lightSpaceMatrix", level->shadows[0]->GetRenderCam()->GetPVMatrix());

        level->shadows[0]->GetRenderCam()->view = glm::lookAt(level->shadows[0]->GetRenderCam()->transform->position, glm::vec3(0.f), level->shadows[0]->GetRenderCam()->transform->up);

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