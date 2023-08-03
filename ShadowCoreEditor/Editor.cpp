#include "Editor.h"
using namespace SC;

void CustomPreRender(Engine* engine) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (Editor::viewport->size_x > 0 && Editor::viewport->size_y > 0)
        Editor::viewport->Render();

    engine->Tick();
}

void Editor::PickingPhase() {
    if (pickingTexture != nullptr) {
        std::cout << "PickingPhase()!\n";
        pickingTexture->Bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (std::shared_ptr<Object> object : Core::Engine->level->objects) {
            if (object->type == MESH) {
                std::shared_ptr<RenderObject> rd = std::dynamic_pointer_cast<RenderObject>(object);

                pickingShader->Activate();
                pickingShader->setUInt("ObjectIndex", 1);
                pickingShader->setUInt("DrawIndex", 1);

                rd->Render(pickingShader);
            }
        }

        pickingTexture->UnBind();
    }
}

void CustomPostRender(Engine* engine) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    bool isDummy = false;


    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());


    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        // Disabling fullscreen would allow the window to be moved to the front of other windows,
        // which we can't undo at the moment without finer window depth/z control.
        ImGui::MenuItem("isDummy", NULL, &isDummy);
        ImGui::Separator();
        ImGui::EndMenu();
    }

    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");


    POINT p;
    if (GetCursorPos(&p)) {
        Editor::MousePos.x = p.x - ImGui::GetWindowContentRegionMin().x - ImGui::GetWindowPos().x;
        Editor::MousePos.y = p.y - ImGui::GetWindowContentRegionMin().y - ImGui::GetWindowPos().y;
    }

    ImVec2 view = ImGui::GetContentRegionAvail();
    if (view.x != Editor::viewport->size_x || view.y != Editor::viewport->size_y)
    {
        if (view.x == 0 || view.y == 0)
        {
            // The window is too small or collapsed.
            goto _skip_render;
        }

        // The window state has been successfully changed.
        glViewport(0, 0, view.x, view.y);
        Editor::viewport->RecreateFB(view.x, view.y);
        Editor::pickingTexture->RecreateFB(view.x, view.y);
        engine->level->main_cam->UpdateProjection(view.x, view.y);
    }

    ImGui::Image((void*)Editor::viewport->GetTextureID(), ImVec2(Editor::viewport->size_x, Editor::viewport->size_y), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
    engine->level->main_cam->SetActive(ImGui::IsItemHovered());
    if (engine->level->main_cam->cam_move_type == FLY_CAM && Core::isEnableEditor && !ImGui::IsWindowDocked()) {
        ImVec2 vMin0 = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax0 = ImGui::GetWindowContentRegionMax();

        vMin0.x += ImGui::GetWindowPos().x;
        vMin0.y += ImGui::GetWindowPos().y;
        vMax0.x += ImGui::GetWindowPos().x;
        vMax0.y += ImGui::GetWindowPos().y;

        ImVec2 vMiddle0 = vMin0.Add(vMax0).Divide(2);

        std::dynamic_pointer_cast<FlyCamera>(engine->level->main_cam)->isWindowDocked = true;
        std::dynamic_pointer_cast<FlyCamera>(engine->level->main_cam)->lastMouseX = (int)vMiddle0.x;
        std::dynamic_pointer_cast<FlyCamera>(engine->level->main_cam)->lastMouseY = (int)vMiddle0.y;
    }
    else if (engine->level->main_cam->cam_move_type == FLY_CAM && Core::isEnableEditor) {
        ImVec2 viewMin = ImGui::GetWindowPos(); // view;
        ImVec2 viewCenter = viewMin.Add(view).Add(viewMin).Divide(2);

        std::dynamic_pointer_cast<FlyCamera>(engine->level->main_cam)->isWindowDocked = false;
        std::dynamic_pointer_cast<FlyCamera>(engine->level->main_cam)->lastMouseX = (int)viewCenter.x;
        std::dynamic_pointer_cast<FlyCamera>(engine->level->main_cam)->lastMouseY = (int)viewCenter.y;
    }
_skip_render:


    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::Begin("Hierarhy");
    for (std::shared_ptr<Object> obj : engine->level->objects) {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;
        if (obj->getId() == Editor::selected_ObjectID)
            flags |= ImGuiTreeNodeFlags_Selected;
        if (ImGui::TreeNodeEx(obj->name.c_str(), flags)) {
            // foreach child
            // ...
            if (ImGui::IsItemClicked(0)) {
                std::cout << obj->name.c_str() << std::endl;
                Editor::selected_ObjectID = obj->getId();
                Core::selected_ObjectID = obj->getId();
            }
            ImGui::TreePop();
        }
        
    }
    ImGui::End();
    /*ImGui::Begin("Hello, world!");

    ImGui::BeginChild("Viewport");
    ImVec2 wsize = ImGui::GetWindowSize();
    std::shared_ptr<Mesh> obj1 = std::dynamic_pointer_cast<Mesh>(level->objects[0]);
    std::shared_ptr<SMaterial> mat = std::dynamic_pointer_cast<SMaterial>(obj1->material);
    float minXY = std::min(wsize.x, wsize.y);
    ImGui::Image((ImTextureID)mat->diffuse_texture->GetTextureID(), { minXY, minXY }, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::EndChild();
    //ImGui::SliderFloat3("Color", (float*)&color, 0., 1.);
    ImGui::End();*/


    bool isDemo = true;
    ImGui::ShowDemoWindow(&isDemo);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

}

void Engine::PostInit() {
    Core::isEnableEditor = true;
    Editor::viewport = std::make_shared<RenderTexture>(level->main_cam, 1, 1);
    Editor::pickingTexture = std::make_shared<PickingTexture>(1, 1);
    Editor::pickingShader = std::make_shared<Shader>("Picking");
    Engine::SetPreRenderPtr(std::function<void(Engine*)>(CustomPreRender));
    Engine::SetPostRenderPtr(std::function<void(Engine*)>(CustomPostRender));

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplGlfw_InitForOpenGL(window.GLFW_window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

static bool Clicked = false;
void Engine::InputProcess() {
    if (glfwGetKey(window.GLFW_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window.GLFW_window, true);

    /*auto& io = ImGui::GetIO();
    if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
        return;
    }*/

    if (Core::isEnableEditor) {
        if (GetAsyncKeyState(VK_LBUTTON)) {
            PickingTexture::PixelInfo Pixel = Editor::pickingTexture->ReadPixel(Editor::MousePos.x, Editor::pickingTexture->size_y - Editor::MousePos.y);
            if (Pixel.ObjectID != 0) {
                Editor::selected_ObjectID = Pixel.ObjectID;
                Core::selected_ObjectID = Pixel.ObjectID;
                std::cout << Pixel.ObjectID << std::endl;
            }
        }
    }

    /*if (glfwGetMouseButton(window.GLFW_window, 0) == GLFW_PRESS && !Clicked) {
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
    }*/
}