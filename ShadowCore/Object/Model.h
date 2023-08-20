#pragma once
#include "ssi.h"
#include "Object/Mesh.h"
#include "Helper/ModelLoading.h"

namespace SC {
    class Model : public Mesh {
    private:
        void LoadModel(std::string _path);
        std::shared_ptr<Object> ProcessNode(aiNode* node, const aiScene* scene, glm::mat4 RootM = glm::mat4(1.0f));
        std::pair<std::shared_ptr<SMaterial>, std::shared_ptr<GeometryData>> ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<std::shared_ptr<Texture>> ProcessTextures(aiMaterial* mat, aiTextureType type, TextureTypes typeName);
    public:
        void Render() override;

        Model(std::string _path, UseLessType _u_type);
    };
};