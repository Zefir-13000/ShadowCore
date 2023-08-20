#include "Editor.h"
using namespace SC;

void Editor::PickingPhase() {
    if (pickingTexture != nullptr) {
        pickingTexture->Bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (std::shared_ptr<Object> object : Core::Engine->level->objects) {
            if (object->type == MESH) {
                std::shared_ptr<RenderObject> rd = std::dynamic_pointer_cast<RenderObject>(object);

                pickingShader->Activate();
                pickingShader->setValue("ObjectIndex", static_cast<uint32_t>(rd->getId()));

                rd->Render(pickingShader);
            }
        }

        glClear(GL_DEPTH_BUFFER_BIT);
        std::shared_ptr<Object> ref_object = Core::Engine->level->GetObjectByID(Editor::selected_ObjectID);
        if (ref_object != nullptr && ref_object->IsRenderAble()) {
            std::shared_ptr<RenderObject> robject = std::dynamic_pointer_cast<RenderObject>(ref_object);
            Editor::arrow_x->transform->Translate(robject->transform->position);
            Editor::arrow_y->transform->Translate(robject->transform->position);
            Editor::arrow_z->transform->Translate(robject->transform->position);

            pickingShader->Activate();

            std::static_pointer_cast<RenderObject>(Editor::arrow_x)->Render(pickingShader, true);
            std::static_pointer_cast<RenderObject>(Editor::arrow_y)->Render(pickingShader, true);
            std::static_pointer_cast<RenderObject>(Editor::arrow_z)->Render(pickingShader, true);
        }

        pickingTexture->UnBind();
    }
}

void Editor::EditorObjectsInit() {
    if (Core::IsInited()) {
        arrow_x = std::make_shared<Arrow>(1);
        arrow_y = std::make_shared<Arrow>(1);
        arrow_z = std::make_shared<Arrow>(1);

        arrow_x->transform->Scale(glm::vec3(0.5f));
        arrow_y->transform->Scale(glm::vec3(0.5f));
        arrow_z->transform->Scale(glm::vec3(0.5f));

        arrow_x->SetName("ArrowX");
        arrow_y->SetName("ArrowY");
        arrow_z->SetName("ArrowZ");

        arrow_x->transform->Rotate(glm::vec3(0.f, 0.f, 90.f));
        arrow_z->transform->Rotate(glm::vec3(90.f, 0.f, 0.f));
        arrow_x->shader_input = std::make_shared<ShaderInputCollection>(arrow_x->render_shader);
        arrow_x->shader_input->AddInput("color", glm::vec3(1.f, 0.f, 0.f));
        arrow_y->shader_input = std::make_shared<ShaderInputCollection>(arrow_y->render_shader);
        arrow_y->shader_input->AddInput("color", glm::vec3(0.f, 1.f, 0.f));
        arrow_z->shader_input = std::make_shared<ShaderInputCollection>(arrow_z->render_shader);
        arrow_z->shader_input->AddInput("color", glm::vec3(0.f, 0.f, 1.f));
    }
}

void CustomPreRender(Engine* engine) {
    glCullFace(GL_FRONT);
    for (std::shared_ptr<ShadowMapTexture> shadowMap : engine->level->shadows) {
        Core::Engine->shadow_shader->Activate();
        Core::Engine->shadow_shader->setValue("lightSpaceMatrix", shadowMap->GetRenderCam()->GetPVMatrix());
        shadowMap->Render(5);
    }
    glCullFace(GL_BACK);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (Editor::viewport->size_x > 0 && Editor::viewport->size_y > 0) {
        Editor::viewport->Bind();
        Editor::viewport->Render(false);

        if (Core::isEnableEditor) {
            glClear(GL_DEPTH_BUFFER_BIT);
            std::shared_ptr<Object> ref_object = engine->level->GetObjectByID(Editor::selected_ObjectID);
            if (ref_object != nullptr && ref_object->IsRenderAble()) {
                std::shared_ptr<RenderObject> robject = std::dynamic_pointer_cast<RenderObject>(ref_object);
                Editor::arrow_x->transform->Translate(robject->transform->position);
                Editor::arrow_y->transform->Translate(robject->transform->position);
                Editor::arrow_z->transform->Translate(robject->transform->position);

                Editor::arrow_x->Render();
                Editor::arrow_y->Render();
                Editor::arrow_z->Render();
            }
        }
        Editor::viewport->UnBind();
    }

    engine->Tick();
}

bool OpenNodesToSelected(std::shared_ptr<Object> obj, const uint32_t selectedID) {
    if (obj->getId() == selectedID) {
        ImGui::SetNextItemOpen(true);
        return true; // Stop traversal when the selected object is found
    }

    if (obj->IsRenderAble()) {
        for (std::shared_ptr<Object> child : std::dynamic_pointer_cast<RenderObject>(obj)->transform->children) {
            if (OpenNodesToSelected(child, selectedID))
                return true;
        }
    }

    ImGui::SetNextItemOpen(false);
    return false;
}

void DrawHierarhy(std::shared_ptr<Object> obj) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
    if (obj->getId() == Editor::selected_ObjectID) {
        flags |= ImGuiTreeNodeFlags_Selected;
        ImGui::SetNextItemOpen(true);
    }
    else
        OpenNodesToSelected(obj, Editor::selected_ObjectID);
    if (obj->IsRenderAble()) {
        std::shared_ptr<RenderObject> Robj = std::dynamic_pointer_cast<RenderObject>(obj);
        if (Robj->transform->children.size() == 0)
            flags |= ImGuiTreeNodeFlags_Leaf;
        bool treeOpen = ImGui::TreeNodeEx(obj->name.c_str(), flags);

        if (ImGui::IsItemClicked(0)) {
            std::cout << obj->name.c_str() << std::endl;
            Editor::prevSelected_ObjectID = Editor::selected_ObjectID;
            Editor::selected_ObjectID = obj->getId();
            Editor::selected_Object = Core::Engine->level->GetObjectByID(Editor::selected_ObjectID);
            Editor::objectNameEdited = false;
            strcpy_s(Editor::objectNameBuffer, Editor::selected_Object->name.c_str());
        }

        if (flags & ImGuiTreeNodeFlags_Selected) {
            ImGui::SetScrollHereY();
        }

        if (treeOpen) {
            for (std::shared_ptr<Object> child : Robj->transform->children) {
                DrawHierarhy(child); // Recursively draw children
            }

            ImGui::TreePop();
        }
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
        Editor::MousePos.x = p.x - static_cast<uint32_t>(ImGui::GetWindowContentRegionMin().x) - static_cast<uint32_t>(ImGui::GetWindowPos().x);
        Editor::MousePos.y = p.y - static_cast<uint32_t>(ImGui::GetWindowContentRegionMin().y) - static_cast<uint32_t>(ImGui::GetWindowPos().y);
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
        glViewport(0, 0, static_cast<GLsizei>(view.x), static_cast<GLsizei>(view.y));
        Editor::viewport->RecreateFB(static_cast<uint32_t>(view.x), static_cast<uint32_t>(view.y));
        Editor::pickingTexture->RecreateFB(static_cast<uint32_t>(view.x), static_cast<uint32_t>(view.y));
        engine->level->main_cam->UpdateProjection(static_cast<int>(view.x), static_cast<int>(view.y));
    }

    ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(Editor::viewport->GetTextureID())), ImVec2(static_cast<float>(Editor::viewport->size_x), static_cast<float>(Editor::viewport->size_y)), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
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



    ImGui::Begin("Inspector");
    if (Editor::selected_Object != nullptr) {
        if (ImGui::InputText("Object Name", Editor::objectNameBuffer, MAXLEN_OBJECT_NAME)) {
            Editor::objectNameEdited = true;
        }
        if (Editor::objectNameEdited && ImGui::IsItemDeactivatedAfterEdit()) {
            Editor::objectNameEdited = false;
            Editor::selected_Object->name = Editor::objectNameBuffer;
        }

        ImGui::InputFloat3("Position", &std::dynamic_pointer_cast<RenderObject>(Editor::selected_Object)->transform->position.x);
        ImGui::InputFloat3("Rotation", &std::dynamic_pointer_cast<RenderObject>(Editor::selected_Object)->transform->rotation.x);
        ImGui::InputFloat3("Scale", &std::dynamic_pointer_cast<RenderObject>(Editor::selected_Object)->transform->scale.x);

        std::dynamic_pointer_cast<RenderObject>(Editor::selected_Object)->transform->Update();
    }
    ImGui::End();

    ImGui::Begin("Hierarhy");
    for (std::shared_ptr<Object> obj : engine->level->objects) {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
        if (obj->getId() == Editor::selected_ObjectID) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }
        else
            OpenNodesToSelected(obj, Editor::selected_ObjectID);
        if (obj->IsRenderAble()) {
            std::shared_ptr<RenderObject> Robj = std::dynamic_pointer_cast<RenderObject>(obj);
            if (Robj->transform->children.size() == 0)
                flags |= ImGuiTreeNodeFlags_Leaf;
            bool treeOpen = ImGui::TreeNodeEx(obj->name.c_str(), flags);

            if (ImGui::IsItemClicked(0)) {
                std::cout << obj->name.c_str() << std::endl;
                Editor::prevSelected_ObjectID = Editor::selected_ObjectID;
                Editor::selected_ObjectID = obj->getId();
                Editor::selected_Object = Core::Engine->level->GetObjectByID(Editor::selected_ObjectID);
                Editor::objectNameEdited = false;
                strcpy_s(Editor::objectNameBuffer, Editor::selected_Object->name.c_str());
            }

            if (flags & ImGuiTreeNodeFlags_Selected) {
                ImGui::SetScrollHereY();
            }

            if (treeOpen) {
                for (std::shared_ptr<Object> child : Robj->transform->children) {
                    DrawHierarhy(child); // Recursively draw children
                }
                
                ImGui::TreePop();
            }
        }
        
        
    }
    ImGui::End();

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


    if (GetAsyncKeyState(VK_LBUTTON) && !Clicked) {
        Clicked = true;

        Editor::PickingPhase();

        PickingTexture::PixelInfo Pixel = Editor::pickingTexture->ReadPixel(Editor::MousePos.x, Editor::pickingTexture->size_y - Editor::MousePos.y);
        if (Pixel.ObjectID != 0) {
            if (Pixel.ObjectID == Editor::arrow_x->getId()) {
                Editor::arrow_dragged = true;
                Editor::selected_arrow = Editor::arrow_x;
                Editor::selected_arrow->shader_input->AddInput("color", glm::vec3(1.0f, 1.0f, 0.0f));
                Editor::start_objectDrag = std::dynamic_pointer_cast<RenderObject>(Editor::selected_Object)->transform->position;
            }
            else if (Pixel.ObjectID == Editor::arrow_y->getId()) {
                Editor::arrow_dragged = true;
                Editor::selected_arrow = Editor::arrow_y;
                Editor::selected_arrow->shader_input->AddInput("color", glm::vec3(1.0f, 1.0f, 0.0f));
                Editor::start_objectDrag = std::dynamic_pointer_cast<RenderObject>(Editor::selected_Object)->transform->position;
            }
            else if (Pixel.ObjectID == Editor::arrow_z->getId()) {
                Editor::arrow_dragged = true;
                Editor::selected_arrow = Editor::arrow_z;
                Editor::selected_arrow->shader_input->AddInput("color", glm::vec3(1.0f, 1.0f, 0.0f));
                Editor::start_objectDrag = std::dynamic_pointer_cast<RenderObject>(Editor::selected_Object)->transform->position;
            }
            else {

                Editor::prevSelected_ObjectID = Editor::selected_ObjectID;
                Editor::selected_ObjectID = Pixel.ObjectID;
                Editor::selected_Object = Core::Engine->level->GetObjectByID(Editor::selected_ObjectID);
                Editor::objectNameEdited = false;

                if (Editor::selected_Object) {
                    strcpy_s(Editor::objectNameBuffer, Editor::selected_Object->name.c_str());
                }
            }
        }
    }
    else if (GetAsyncKeyState(VK_LBUTTON) && Clicked) {
        // handle arrows drag
        if (Editor::arrow_dragged) {
            glm::mat4 cam_view = Core::Engine->level->main_cam->view;
            glm::mat4 cam_proj = Core::Engine->level->main_cam->proj;

            glm::mat4 cam_inv_view = glm::inverse(cam_view);
            glm::mat4 cam_inv_proj = glm::inverse(cam_proj);

            glm::vec4 objectViewSpacePos = cam_view * glm::vec4(Editor::start_objectDrag, 1.0f);

            float mouse_x = static_cast<float>(Editor::MousePos.x);
            float mouse_y = static_cast<float>(Editor::MousePos.y);

            float ndc_x = (2.0f * mouse_x) / Editor::pickingTexture->size_x - 1.0f;
            float ndc_y = 1.0f - (2.0f * mouse_y) / Editor::pickingTexture->size_y;

            float focal_lenght = 1.0f / tanf(glm::radians(Core::Engine->level->main_cam->FOV / 2.0f));
            float ar = (float)Editor::pickingTexture->size_y / (float)Editor::pickingTexture->size_x;
            glm::vec3 ray_view(-ndc_x / focal_lenght, (-ndc_y * ar) / focal_lenght, 1.0f);

            glm::vec4 view_space_intersect = glm::vec4(glm::vec3(ray_view) * objectViewSpacePos.z, 1.0f);
            glm::vec4 point_world = cam_inv_view * view_space_intersect;
            glm::vec3 newPosition = glm::vec3(0.f);
            if (Editor::arrow_x == Editor::selected_arrow) {
                newPosition = glm::vec3(point_world.x, Editor::start_objectDrag.y, Editor::start_objectDrag.z);
            }
            else if (Editor::arrow_y == Editor::selected_arrow) {
                newPosition = glm::vec3(Editor::start_objectDrag.x, point_world.y, Editor::start_objectDrag.z);
            }
            else if (Editor::arrow_z == Editor::selected_arrow) {
                newPosition = glm::vec3(Editor::start_objectDrag.x, Editor::start_objectDrag.y, point_world.z);
            }
            std::dynamic_pointer_cast<RenderObject>(Editor::selected_Object)->transform->Translate(newPosition);
            std::dynamic_pointer_cast<RenderObject>(Editor::selected_Object)->UpdateComponents();
        }
    }
    else if (!GetAsyncKeyState(VK_LBUTTON) && Clicked) {
        Clicked = false;
        if (Editor::arrow_dragged) {
            Editor::arrow_dragged = false;
            Editor::arrow_x->shader_input->AddInput("color", glm::vec3(1.0f, 0.0f, 0.0f));
            Editor::arrow_y->shader_input->AddInput("color", glm::vec3(0.0f, 1.0f, 0.0f));
            Editor::arrow_z->shader_input->AddInput("color", glm::vec3(0.0f, 0.0f, 1.0f));
            Editor::selected_arrow = nullptr;
        }
    }

    /*if (glfwGetMouseButton(window.GLFW_window, 0) == GLFW_PRESS && !Clicked) {
        Clicked = true;
        double mouseX;
        double mouseY;
        glfwGetCursorPos(window.GLFW_window, &mouseX, &mouseY);
        glm::vec3 ray_dir = Ray::GetScreenToWorld(glm::vec2(mouseX, mouseY), glm::vec2(window.width, window.height), level->main_cam->proj, level->main_cam->view, level->main_cam->transform->position);
        std::shared_ptr<Ray> ray = std::make_shared<Ray>(level->main_cam->transform->position, ray_dir);
        std::shared_ptr<Mesh> obj1 = std::dynamic_pointer_cast<Mesh>(level->objects[0]);

        float t = 0;
        std::shared_ptr<AABB> aabb = obj1->GetComponent<AABB>();
        if (aabb != nullptr) {
            //std::cout << "AABB - min: X: " << obj1->aabb_box->box.min.x << " Y: " << obj1->aabb_box->box.min.y << " Z: " << obj1->aabb_box->box.min.z << " max: X: " << obj1->aabb_box->box.max.x << " Y: " << obj1->aabb_box->box.max.y << " Z: " << obj1->aabb_box->box.max.z << std::endl;
            std::cout << (t = ray->RayIntersectsAABB(aabb->box, obj1->transform->model)) << std::endl;
            //std::cout << "Ray - X: " << ray_dir.x << " Y: " << ray_dir.y << " Z: " << ray_dir.z << std::endl;
            //std::cout << "CamRay - X: " << level->main_cam->transform->forward.x << " Y: " << level->main_cam->transform->forward.y << " Z: " << level->main_cam->transform->forward.z << std::endl;
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