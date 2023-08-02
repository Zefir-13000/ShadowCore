#pragma once
#include "ssi.h"
#include "Object/Object.h"
#include "Common/Shader.h"
#include "Common/Transform.h"
#include "Common/Texture.h"
#include "Common/Material.h"
#include "Object/RenderObject.h"
#include "Component/RayObjects.h"

namespace SC {

    class Mesh : public RenderObject {
    private:
        // none
    protected:
        // none
    public:
        MeshType meshType = MESH_TYPE;

        void Render() override;

        Mesh(std::string _name, std::shared_ptr<GeometryData> _geom_data, UseLessType _u_type);

        void SetMaterial(std::shared_ptr<Material> _material);

        std::shared_ptr<Material> material;
    };

};

