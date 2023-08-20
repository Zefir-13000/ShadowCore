#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform sampler2D diffuse_texture;
uniform int has_diffuse_texture;

uniform sampler2D specular_texture;
uniform int has_specular_texture;

uniform sampler2D emission_texture;
uniform int has_emission_texture;

uniform sampler2D shadow_map;
uniform int has_shadow_map;

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

float SampleShadowMap(vec2 coords, float compare)
{
    if(compare > 1.0)
        return 1.0;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float bias = max(0.0005 * (1.0 - dot(norm, lightDir)), 0.00005);
    float closestDepth = texture(shadow_map, coords.xy).r;
    float currentDepth = compare;
    float shadow = 1.0;
    if (currentDepth - bias > closestDepth) {
        shadow = 0.2;
    }

    return shadow;
}
float SampleShadowMapLinear(vec2 coords, float compare, vec2 resolution, vec2 texelSize)
{
    vec2 pixelPos = coords / texelSize;
    vec2 fracPart = fract(pixelPos);
    vec2 startTexel = (pixelPos - fracPart) * texelSize;

    float blTexel = SampleShadowMap(startTexel, compare);
    float brTexel = SampleShadowMap(startTexel + vec2(texelSize.x, 0.0), compare);
    float tlTexel = SampleShadowMap(startTexel + vec2(0.0, texelSize.y), compare);
    float trTexel = SampleShadowMap(startTexel + texelSize, compare);

    float mixA = mix(blTexel, tlTexel, fracPart.y);
    float mixB = mix(brTexel, trTexel, fracPart.y);

    return mix(mixA, mixB, fracPart.x);
}
float SampleShadowMapPCF(vec2 coords, float compare, vec2 resolution, vec2 texelSize, int filterSize)
{
    float NUM_SAMPLES = float(filterSize);
    float SAMPLES_START = (NUM_SAMPLES - 1.0f) / 2.0f;
    float NUM_SAMPLES_SQUARED = NUM_SAMPLES * NUM_SAMPLES;

    float result = 0.0f;
    for (float y = -SAMPLES_START; y <= SAMPLES_START; y += 1.0f)
    {
        for (float x = -SAMPLES_START; x <= SAMPLES_START; x += 1.0f)
        {
            vec2 coordsOffset = vec2(x, y) * texelSize;
            result += SampleShadowMapLinear(coords + coordsOffset, compare, resolution, texelSize);
        }
    }
    return result / NUM_SAMPLES_SQUARED;
}


void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = vec3(1);
    vec3 ambient = vec3(0.1);
    vec3 specular = vec3(0.5);
    vec3 emission = vec3(0.0);
    float shadow = 1.0;
    if (has_diffuse_texture == 1) {
        vec4 diffuse_tex = texture(diffuse_texture, TexCoords);
        if (diffuse_tex.a == 0.0) {
            discard;
        }
        diffuse = light.DiffuseStrength * diff * vec3(diffuse_tex); 
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

    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    shadow = SampleShadowMapPCF(projCoords.xy, projCoords.z, textureSize(shadow_map, 0), 1.0 / textureSize(shadow_map, 0), 5);
    

    FragColor = vec4(ambient + shadow * (diffuse + specular) + emission, 1.0);
} 