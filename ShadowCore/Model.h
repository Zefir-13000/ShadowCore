#pragma once
#include "ssi.h"
#include "Mesh.h"

namespace SC {
    class Model : public Mesh {
    private:
        // none
    public:
        void Render() override;

        Model(std::string _name, std::vector<float>& _vertices);
        Model(std::string _name, std::vector<Vertex>& _vertices);

        Model(std::string _name, std::vector<float> _vertices, UseLessType _u_type);
        Model(std::string _name, std::vector<Vertex> _vertices, UseLessType _u_type);
    };
};