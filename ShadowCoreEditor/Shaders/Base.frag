#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D diffuse_texture;
uniform int has_diffuse_texture;

uniform sampler2D specular_texture;
uniform int has_specular_texture;

uniform vec3 viewPos;

struct Light {
    vec3 position;
    vec3 color;
};

struct Material {
    vec3 Albedo;
    float Ambient;
    float Shininess;
    float SpecularStrength;
};

uniform Light light;
uniform Material material;

void main()
{
    vec3 ambient = material.Ambient * light.color;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    vec3 specular = material.SpecularStrength * spec * light.color;  

    if (has_diffuse_texture == 1)
        FragColor = texture(diffuse_texture, TexCoords) * vec4(material.Albedo, 1.0);
    else {
        
        FragColor = vec4(material.Albedo * (ambient + diffuse + specular), 1.0);

    }
} 