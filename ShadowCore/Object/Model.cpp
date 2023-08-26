#include "Model.h"
#include "Core/Core.h"

using namespace SC;

Model::Model(std::string _path, UseLessType _u_type) : Mesh(_path, std::make_shared<GeometryData>(), _u_type) {
    Model::type = MESH;
    Model::meshType = MODEL_TYPE;

    Model::LoadModel(_path);
}

void Model::LoadModel(std::string _path) {
    Assimp::Importer import;

    const aiScene* scene = import.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    
    Model::name = scene->mRootNode->mName.C_Str();
    Model::ProcessNode(scene->mRootNode, scene);

    Model::transform->Update();
}

std::shared_ptr<Object> Model::ProcessNode(aiNode* node, const aiScene* scene, glm::mat4 RootM) {
    glm::mat4 nodeMat = aiMatrix4x4ToGlm(&node->mTransformation);
    glm::mat4 nexttranslate = nodeMat * RootM;

    std::vector<std::shared_ptr<GeometryData>> meshData = {};
    std::shared_ptr<SMaterial> meshMaterial = nullptr;
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::pair<std::shared_ptr<SMaterial>, std::shared_ptr<GeometryData>> meshDataPair = ProcessMesh(mesh, scene);
        meshMaterial = meshDataPair.first;
        meshData.push_back(meshDataPair.second); // RenderSeq
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        transform->AddChild(ProcessNode(node->mChildren[i], scene, nexttranslate));
    }

    std::shared_ptr<RenderSequence> renderSeq = std::make_shared<RenderSequence>(meshData);
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(node->mName.C_Str(), renderSeq, MANDATORY);
    mesh->transform->Translate(glm::vec3(nexttranslate[3]));
    mesh->transform->Rotate(glm::eulerAngles(glm::quat(nexttranslate)));
    mesh->transform->Scale(glm::vec3(glm::length(glm::vec3(nexttranslate[0])), glm::length(glm::vec3(nexttranslate[1])), glm::length(glm::vec3(nexttranslate[2]))));
    
    mesh->transform->LocalTranslate(glm::vec3(nodeMat[3]));
    mesh->transform->LocalRotate(glm::eulerAngles(glm::quat(nodeMat)));
    mesh->transform->LocalScale(glm::vec3(glm::length(glm::vec3(nodeMat[0])), glm::length(glm::vec3(nodeMat[1])), glm::length(glm::vec3(nodeMat[2]))));

    mesh->SetShader(Core::Engine->GetDefaultShader());
    mesh->SetMaterial(meshMaterial);
    return mesh;
}

std::pair<std::shared_ptr<SMaterial>, std::shared_ptr<GeometryData>> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::shared_ptr<SMaterial> Smaterial = std::make_shared<SMaterial>();

    if (mesh->mNumVertices == 0) {
        return std::make_pair(Smaterial, std::make_shared<GeometryData>());
    }

    std::shared_ptr<std::vector<Vertex>> vertices = std::make_shared<std::vector<Vertex>>(std::initializer_list<Vertex>{});
    std::shared_ptr < std::vector<uint32_t>> indices = std::make_shared<std::vector<uint32_t>>(std::initializer_list<uint32_t>{});

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};
        glm::vec3 data = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.position = data;
        
        if (mesh->HasNormals()) {
            data = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            vertex.normal = data;
        }

        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec = glm::vec2(1.0f);
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoord = vec;
        }
        else
            vertex.texCoord = glm::vec2(0.0f, 0.0f);

        vertices->push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        if (face.mNumIndices < 3) {
            continue;
        }
        assert(face.mNumIndices == 3);
        for (uint32_t j = 0; j < face.mNumIndices; j++)
            indices->push_back(face.mIndices[j]);
    }

    
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<std::shared_ptr<Texture>> diffuseMaps = ProcessTextures(material,
            aiTextureType_DIFFUSE, DIFFUSE_TEXTURE);
        if (diffuseMaps.size() > 0) Smaterial->diffuse_texture = diffuseMaps[0];
        std::vector<std::shared_ptr<Texture>> specularMaps = ProcessTextures(material,
            aiTextureType_SPECULAR, SPECULAR_TEXTURE);
        if (specularMaps.size() > 0) Smaterial->specular_texture = specularMaps[0];
    }

    return std::make_pair(Smaterial, std::make_shared<GeometryData>(vertices, indices));
}

std::vector<std::shared_ptr<Texture>> Model::ProcessTextures(aiMaterial* mat, aiTextureType type, TextureTypes typeName) {
    std::vector<std::shared_ptr<Texture>> textures = {};
    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::shared_ptr<Texture> tex = std::make_shared<Texture>(str.C_Str(), typeName);
        std::cout << str.C_Str() << std::endl;

        textures.push_back(tex);
    }

    return textures;
}

void Model::Render() {
    //if (Core::isEnableEditor) //  && this->getId() == Core::selected_ObjectID
    //    RenderComponents();
    if (Model::render_data->render_shader != nullptr) {
        Model::render_data->render_shader->Activate();

        if (Model::material != nullptr && Model::material->material_type == MATERIAL) {
            std::shared_ptr<SMaterial> matColor = std::dynamic_pointer_cast<SMaterial>(Model::material);

            Model::render_data->render_shader->setValue("material.Ambient", matColor->Ambient);
            Model::render_data->render_shader->setValue("material.Diffuse", matColor->Diffuse);
            Model::render_data->render_shader->setValue("material.Specular", matColor->Specular);
            Model::render_data->render_shader->setValue("material.Emission", matColor->Emission);
            Model::render_data->render_shader->setValue("material.Shininess", matColor->Shininess);

            if (matColor->diffuse_texture != nullptr) {
                Model::render_data->render_shader->setValue("has_diffuse_texture", 1);
                Model::render_data->render_shader->setValue("diffuse_texture", 0);
                matColor->diffuse_texture->Bind(Model::render_data->render_shader, 0);
            }
            else {
                Model::render_data->render_shader->setValue("has_diffuse_texture", 0);
            }
            if (matColor->specular_texture != nullptr) {
                Model::render_data->render_shader->setValue("has_specular_texture", 1);
                Model::render_data->render_shader->setValue("specular_texture", 1);
                matColor->specular_texture->Bind(Model::render_data->render_shader, 1);
            }
            else {
                Model::render_data->render_shader->setValue("has_specular_texture", 0);
            }
            if (matColor->emission_texture != nullptr) {
                Model::render_data->render_shader->setValue("has_emission_texture", 1);
                Model::render_data->render_shader->setValue("emission_texture", 2);
                matColor->emission_texture->Bind(Model::render_data->render_shader, 2);
            }
            else {
                Model::render_data->render_shader->setValue("has_emission_texture", 0);
            }
        }
        else {
            Model::render_data->render_shader->setValue("material.Ambient", glm::vec3(0.1f));
            Model::render_data->render_shader->setValue("material.Diffuse", glm::vec3(1.f));
            Model::render_data->render_shader->setValue("material.Specular", glm::vec3(0.5f));
            Model::render_data->render_shader->setValue("material.Emission", glm::vec3(0.0f));
            Model::render_data->render_shader->setValue("material.Shininess", 32.f);

            Model::render_data->render_shader->setValue("has_diffuse_texture", 0);
            Model::render_data->render_shader->setValue("has_specular_texture", 0);
            Model::render_data->render_shader->setValue("has_emission_texture", 0);
        }

        Model::render_data->render_shader->setValue("model", Model::transform->GetMatrix());
        Model::render_data->render_shader->setValue("MVP", Core::Engine->level->main_cam->proj * Core::Engine->level->main_cam->view * Mesh::transform->GetMatrix());

        if (Model::render_data->receive_shadows) {
            Model::render_data->render_shader->setValue("receive_shadows", 1);
        }
        else {
            Model::render_data->render_shader->setValue("receive_shadows", 0);
        }

        for (std::shared_ptr<GeometryData> geom_data : renderSeq->geoms_data) {
            glBindVertexArray(geom_data->VAO);

            if (geom_data->render_type == ELEMENT) {
                glDrawElements(geom_data->render_mode, static_cast<GLsizei>(geom_data->indices_count), GL_UNSIGNED_INT, 0);
            }
            else if (geom_data->render_type == ARRAY) {
                glDrawArrays(geom_data->render_mode, 0, static_cast<GLsizei>(geom_data->vertices_count));
            }
            glBindVertexArray(0);
        }

        for (std::shared_ptr<Object> child : Model::transform->GetChildren()) {
            if (child && child->IsRenderAble()) {
                std::dynamic_pointer_cast<RenderObject>(child)->Render();
            }
        }
    }
    else if (Model::render_data->render_shader == nullptr) {
        std::cerr << "ERROR::MODEL::RENDER_SHADER - RENDER_OBJECT (" << name << ") at 0x" << std::hex << this << " - RENDER_SHADER is NULL" << std::endl;
        Model::render_data->render_shader = enginePtr->GetDefaultShader();
    }
}