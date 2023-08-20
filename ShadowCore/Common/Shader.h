#pragma once
#include "ssi.h"
#include "Utils/STime.h"

namespace SC {

    class Shader {
    public:
        GLuint ID;
        Shader(_In_ std::string name);
        ~Shader();
        void Activate();
        void setValue(const std::string& name, bool value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        void setValue(const std::string& name, int value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setValue(const std::string& name, uint32_t value) const {
            glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setValue(const std::string& name, float value) const {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setValue(const std::string& name, const glm::vec2& value) const {
            glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setValue(const std::string& name, float x, float y) const {
            glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
        }
        void setValue(const std::string& name, const glm::vec3& value) const {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setValue(const std::string& name, float x, float y, float z) const {
            glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
        }
        void setValue(const std::string& name, const glm::vec4& value) const {
            glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setValue(const std::string& name, float x, float y, float z, float w) const {
            glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
        }
        void setValue(const std::string& name, const glm::mat2& mat) const {
            glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        void setValue(const std::string& name, const glm::mat3& mat) const {
            glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        void setValue(const std::string& name, const glm::mat4& mat) const {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
    };

    class ShaderInputBase {
    public:
        ShaderInputBase(const std::string& name) : name(name) {};
        virtual ~ShaderInputBase() {}

        virtual void BindToShader(std::shared_ptr<Shader> _shader) const = 0;

        std::string name;
    };

    template <typename T>
    class ShaderInput : public ShaderInputBase {
    public:
        T value;
        ShaderInput(const std::string& name, const T& value) : ShaderInputBase(name) {
            this->value = value;
        }

        void BindToShader(std::shared_ptr<Shader> _shader) const {
            _shader->setValue(name, value);
        }

        void SetValue(const T& newValue) {
            value = newValue;
        }

    private:
        // none
    };



    class ShaderInputCollection {
    public:
        std::shared_ptr<Shader> shader = nullptr;
        ShaderInputCollection(std::shared_ptr<Shader> _shader) {
            this->shader = _shader;
        }

        template <typename T>
        void AddInput(const std::string& name, const T& value) {
            for (std::shared_ptr<ShaderInputBase> input : inputs) {
                if (input->name == name) {
                    inputs.erase(std::remove(inputs.begin(), inputs.end(), input), inputs.end());
                    break;
                }
            }
            inputs.push_back(std::make_shared<ShaderInput<T>>(name, value));
        }

        void BindAllToShader() const {
            shader->Activate();
            for (std::shared_ptr<ShaderInputBase> input : inputs) {
                input->BindToShader(this->shader);
            }
        }

    private:
        std::vector<std::shared_ptr<ShaderInputBase>> inputs;
    };

};