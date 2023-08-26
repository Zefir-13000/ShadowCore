#include "Object/ShadowMap.h"
#include "Object/Light.h"
#include "Core/Core.h"

using namespace SC;

CascadeShadowMap::CascadeShadowMap(std::shared_ptr<DirectionalLight> _dir_light, uint32_t _size) : dir_light(_dir_light), shadow_map_size(_size) {
    float cameraFarPlane = Core::Engine->level->main_cam->far_plane;
    CascadeShadowMap::shadowCascadeLevels = { cameraFarPlane / 50.0f, cameraFarPlane / 25.0f, cameraFarPlane / 10.0f, cameraFarPlane / 2.0f };

    glGenFramebuffers(1, &framebuffer);
    glGenTextures(1, &depth_tex);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depth_tex);
    glTexImage3D(
        GL_TEXTURE_2D_ARRAY,
        0,
        GL_DEPTH_COMPONENT32F,
        _size,
        _size,
        int(shadowCascadeLevels.size()) + 1,
        0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        nullptr);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_tex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER::CSM Framebuffer is not complete!";
        throw 0;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

CascadeShadowMap::~CascadeShadowMap() {
    glDeleteTextures(1, &depth_tex);
    glDeleteFramebuffers(1, &framebuffer);
}

void CascadeShadowMap::Bind() {
    glActiveTexture(GL_TEXTURE0 + 5);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depth_tex);
}

void CascadeShadowMap::UnBind() {
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projview)
{
    const auto inv = glm::inverse(projview);

    std::vector<glm::vec4> frustumCorners;
    for (unsigned int x = 0; x < 2; ++x)
    {
        for (unsigned int y = 0; y < 2; ++y)
        {
            for (unsigned int z = 0; z < 2; ++z)
            {
                const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                frustumCorners.push_back(pt / pt.w);
            }
        }
    }

    return frustumCorners;
}

glm::mat4 CascadeShadowMap::getLightSpaceMatrix(uint32_t cascade_index, const float nearPlane, const float farPlane)
{
    glm::vec3 frustumCorners[8] = {
                glm::vec3(-1.0f,  1.0f, -1.0f),
                glm::vec3(1.0f,  1.0f, -1.0f),
                glm::vec3(1.0f, -1.0f, -1.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(-1.0f,  1.0f,  1.0f),
                glm::vec3(1.0f,  1.0f,  1.0f),
                glm::vec3(1.0f, -1.0f,  1.0f),
                glm::vec3(-1.0f, -1.0f,  1.0f),
    };

    glm::mat4 invCam = glm::inverse(Core::Engine->level->main_cam->proj * Core::Engine->level->main_cam->view); // CamPers
    for (uint32_t i = 0; i < 8; i++) {
        glm::vec4 invCorner = invCam * glm::vec4(frustumCorners[i], 1.0f);
        frustumCorners[i] = invCorner / invCorner.w;
    }

    for (uint32_t i = 0; i < 4; i++) {
        glm::vec3 dist = frustumCorners[i + 4] - frustumCorners[i];
        frustumCorners[i + 4] = frustumCorners[i] + (dist * (farPlane / Core::Engine->level->main_cam->far_plane));
        frustumCorners[i] = frustumCorners[i] + (dist * (nearPlane / Core::Engine->level->main_cam->far_plane));
    }

    glm::vec3 frustumCenter = glm::vec3(0.0f);
    for (uint32_t i = 0; i < 8; i++) {
        frustumCenter += frustumCorners[i];
    }
    frustumCenter /= 8.0f;

    float radius = 0.0f;
    for (uint32_t i = 0; i < 8; i++) {
        float distance = glm::length(frustumCorners[i] - frustumCenter);
        radius = glm::max(radius, distance);
    }
    radius = std::ceil(radius * 16.0f) / 16.0f;

    glm::vec3 maxExtents = glm::vec3(radius);
    glm::vec3 minExtents = -maxExtents;

    glm::vec3 lightDir = glm::normalize(dir_light->direction);
    glm::mat4 lightViewMatrix = glm::lookAt(frustumCenter + lightDir, frustumCenter, glm::vec3(0.0f, 1.0f, 0.0f));

    minExtents.z /= 2.0f;
    maxExtents.z = (maxExtents.z < 10.f) ? 10.f : maxExtents.z;

    glm::mat4 lightOrthoMatrix = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, minExtents.z, maxExtents.z);

    glm::mat4 shadowMatrix = lightOrthoMatrix * lightViewMatrix;
    glm::vec4 shadowOrigin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    shadowOrigin = shadowMatrix * shadowOrigin;
    float storedW = shadowOrigin.w;
    shadowOrigin = shadowOrigin * (float)shadow_map_size / 2.0f;

    glm::vec4 roundedOrigin = glm::round(shadowOrigin);
    glm::vec4 roundOffset = roundedOrigin - shadowOrigin;
    roundOffset = roundOffset * 2.0f / (float)shadow_map_size;
    roundOffset.z = 0.0f;
    roundOffset.w = 0.0f;

    glm::mat4 shadowProj = lightOrthoMatrix;
    shadowProj[3] += roundOffset;
    lightOrthoMatrix = shadowProj;

    return lightOrthoMatrix * lightViewMatrix;
}

std::vector<glm::mat4> CascadeShadowMap::getLightMatrices() {
    std::vector<glm::mat4> ret;
    for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i)
    {
        if (i == 0)
        {
            ret.push_back(getLightSpaceMatrix(i, Core::Engine->level->main_cam->near_plane, shadowCascadeLevels[i]));
        }
        else if (i < shadowCascadeLevels.size())
        {
            ret.push_back(getLightSpaceMatrix(i, shadowCascadeLevels[i - 1], shadowCascadeLevels[i]));
        }
        else
        {
            ret.push_back(getLightSpaceMatrix(i, shadowCascadeLevels[i - 1], Core::Engine->level->main_cam->far_plane));
        }
    }
    return ret;
}

void CascadeShadowMap::Render() {
    glCullFace(GL_FRONT);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glViewport(0, 0, shadow_map_size, shadow_map_size);
    glClear(GL_DEPTH_BUFFER_BIT);

    Core::Engine->level->Render(Core::Engine->GetDefaultDirectionalShadowShader());

    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, Core::Engine->window.width, Core::Engine->window.height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CascadeShadowMap::Update() {
    Core::Engine->GetDefaultShader()->Activate();
    for (int i = 0; i < shadowCascadeLevels.size(); ++i) {
        Core::Engine->engine_active_shader->setValue("cascadeSplits[" + std::to_string(i) + "]", shadowCascadeLevels[i]);
    }

    std::vector<glm::mat4> lightMats = CascadeShadowMap::getLightMatrices();
    for (int i = 0; i < lightMats.size(); ++i) {
        Core::Engine->engine_active_shader->setValue("lightSpaceMatrices[" + std::to_string(i) + "]", lightMats[i]);
    }
    Core::Engine->GetDefaultDirectionalShadowShader()->Activate();
    for (int i = 0; i < lightMats.size(); ++i) {
        Core::Engine->engine_active_shader->setValue("lightSpaceMatrices[" + std::to_string(i) + "]", lightMats[i]);
    }

    Core::Engine->GetDefaultShader()->Activate();
    Core::Engine->engine_active_shader->setValue("CSM_shadow_map", 5);
    Core::Engine->engine_active_shader->setValue("cascadeCount", static_cast<int>(shadowCascadeLevels.size()));

    CascadeShadowMap::Render();
}