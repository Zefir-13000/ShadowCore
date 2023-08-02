#pragma once
#include "ssi.h"
#include "Object/Mesh.h"

namespace SC {
    class Model : public Mesh {
    private:
        // none
    public:
        void Render() override;

        Model(std::string _name, std::shared_ptr<GeometryData> _geom_data, UseLessType _u_type);
    };
};