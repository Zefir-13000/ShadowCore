#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D diffuse_texture;
uniform int has_diffuse_texture;

uniform sampler2D specular_texture;
uniform int has_specular_texture;

uniform sampler2D emission_texture;
uniform int has_emission_texture;

uniform vec3 viewPos;

// Engine Uniform
uniform float time;

struct Light {
    vec3 position;
    vec3 color;

    vec3 AmbientStrength;
    vec3 DiffuseStrength;
    vec3 SpecularStrength;
};

struct Material {
	vec3 Ambient;
    vec3 Diffuse;
	vec3 Specular;
    vec3 Emission;
    float Shininess;
};

uniform Light light;
uniform Material material;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = vec3(1);
    vec3 ambient = vec3(0.1);
    vec3 specular = vec3(0.5);
    vec3 emission = vec3(0.0);
    if (has_diffuse_texture == 1) {
        diffuse = light.DiffuseStrength * diff * vec3(texture(diffuse_texture, TexCoords)); 
        ambient = light.AmbientStrength * vec3(texture(diffuse_texture, TexCoords));;
    }
    else {
        diffuse = light.DiffuseStrength * (diff * material.Diffuse);
        ambient = light.AmbientStrength * material.Ambient;
    }

    vec3 viewDir = normalize(viewPos - FragPos);

    if (dot(norm, lightDir) > 0) {
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);

        if (has_specular_texture == 1) {
            specular = light.SpecularStrength * spec * vec3(texture(specular_texture, TexCoords));
        }
        else {
            specular = light.SpecularStrength * (spec * material.Specular); 
        }
    }
    else {
        specular = vec3(0);
    }

    if (has_emission_texture == 1) {
        emission = floor(vec3(1.0) - texture(specular_texture, TexCoords).rgb) * texture(emission_texture, TexCoords + vec2(0,time)).rgb;
    }
    else {
        emission = material.Emission;
    }

    FragColor = vec4(ambient + diffuse + specular + emission, 1.0);
} 