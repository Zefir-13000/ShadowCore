#pragma once
#include "ssi.h"
#include "Object.h"
#include "Shader.h"
#include "Transform.h"
#include "Texture.h"
#include "Material.h"
#include "RenderObject.h"
#include "RayObjects.h"

namespace SC {

    class Mesh : public RenderObject {
    private:
        // none
    protected:
        // none
    public:
        MeshType meshType = MESH_TYPE;

        void Render() override;

        Mesh(std::string _name, std::vector<float>& _vertices);
        Mesh(std::string _name, std::vector<Vertex>& _vertices);

        Mesh(std::string _name, std::vector<float> _vertices, UseLessType _u_type);
        Mesh(std::string _name, std::vector<Vertex> _vertices, UseLessType _u_type);

        void SetMaterial(std::shared_ptr<Material> _material);

        std::shared_ptr<Material> material;

        Transform local_transform, transform;
    };

};

