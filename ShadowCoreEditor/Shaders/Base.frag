#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in float ClipZ;

uniform sampler2D diffuse_texture;
uniform int has_diffuse_texture;

uniform sampler2D specular_texture;
uniform int has_specular_texture;

uniform sampler2D emission_texture;
uniform int has_emission_texture;

uniform sampler2DArray CSM_shadow_map;

uniform float farPlane;
uniform vec3 viewPos;
uniform mat4 lightSpaceMatrices[16];
uniform float cascadeSplits[16];

uniform mat4 view;
uniform int cascadeCount;

uniform int receive_shadows;

// Engine Uniform
uniform float time;

struct DirectionalLight {
    vec3 direction;

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

uniform DirectionalLight light;
uniform Material material;

float SampleShadowMap(vec2 coords, float compare, int cascadeIndex)
{
    if(compare > 1.0)
        return 1.0;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction); // light.position - FragPos
    float bias = max(0.001 * (1.0 - dot(norm, lightDir)), 0.0001);
    const float biasModifier = 0.5f;
    if (cascadeIndex == cascadeCount)
    {
        bias *= 2.f;
    }
    else
    {
        bias *= 1.5f;
    }
    float closestDepth = texture(CSM_shadow_map, vec3(coords.xy, cascadeIndex)).r;
    float currentDepth = compare;
    float shadow = 1.0;
    if (currentDepth - bias > closestDepth) {
        shadow = 0.2;
    }

    return shadow;
}
float SampleShadowMapLinear(vec2 coords, float compare, vec2 resolution, vec2 texelSize, int cascadeIndex)
{
    vec2 pixelPos = coords / texelSize;
    vec2 fracPart = fract(pixelPos);
    vec2 startTexel = (pixelPos - fracPart) * texelSize;

    float blTexel = SampleShadowMap(startTexel, compare, cascadeIndex);
    float brTexel = SampleShadowMap(startTexel + vec2(texelSize.x, 0.0), compare, cascadeIndex);
    float tlTexel = SampleShadowMap(startTexel + vec2(0.0, texelSize.y), compare, cascadeIndex);
    float trTexel = SampleShadowMap(startTexel + texelSize, compare, cascadeIndex);

    float mixA = mix(blTexel, tlTexel, fracPart.y);
    float mixB = mix(brTexel, trTexel, fracPart.y);

    return mix(mixA, mixB, fracPart.x);
}
float SampleShadowMapPCF(vec2 coords, float compare, vec2 resolution, vec2 texelSize, int filterSize, int cascadeIndex)
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
            result += SampleShadowMapLinear(coords + coordsOffset, compare, resolution, texelSize, cascadeIndex);
        }
    }
    return result / NUM_SAMPLES_SQUARED;
}

float ShadowCalculation(vec3 fragPosWorldSpace)
{
    vec4 FragPosView = view * vec4(FragPos.xyz, 1.0);
    float depth_val = abs(FragPosView.z);
    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (ClipZ <= cascadeSplits[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }

    vec4 FragPosLightSpace = lightSpaceMatrices[layer] * vec4(fragPosWorldSpace, 1.0);
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 0.0;
    }
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.direction);
    float bias = 0.0005; //max(0.0005 * (1.0 - dot(normal, lightDir)), 0.00005);
    const float biasModifier = 0.5f;
    /*if (layer == cascadeCount)
    {
        bias *= 1 / (100.0 * biasModifier);
    }
    else
    {
        bias *= 1 / (cascadeSplits[layer] * biasModifier);
    }*/

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(CSM_shadow_map, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(CSM_shadow_map, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
        
    return shadow;
}


void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.direction);

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

  
    vec4 FragPosView = view * vec4(FragPos.xyz, 1.0);
    float depth_val = abs(FragPosView.z);
    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (ClipZ <= cascadeSplits[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }

    if (receive_shadows == 1) {
        vec4 FragPosLightSpace = lightSpaceMatrices[layer] * vec4(FragPos, 1.0);
        vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
        projCoords = projCoords * 0.5 + 0.5;
        shadow = SampleShadowMapPCF(projCoords.xy, projCoords.z, textureSize(CSM_shadow_map, 0).xy, 1.0 / textureSize(CSM_shadow_map, 0).xy, 5, layer);
    }

    FragColor = vec4(ambient + shadow * (diffuse + specular) + emission, 1.0);

} 