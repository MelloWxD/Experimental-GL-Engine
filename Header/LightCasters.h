#pragma once
#include"Constants.h"
#include"ShaderModule.h"
#include"FBO.h"
class Renderer;

struct BaseLight
{
    bool _enable = true;
    float diffuseStrength = 1.f;
    float ambientStrength = 0.03f;
    v3 color = v3(1.f);
    virtual void DrawShadowMap(Renderer* pRender, ShaderModule* pShader) = 0;
    virtual void BindShadowMap(ShaderModule* pLightingShader) = 0;
    enum eLightType : unsigned int
    {
        eDirectLight = 0,
        eSpotLight = 1,
        ePointLight = 2
    };
    unsigned _type;
};
struct DirLight : public BaseLight
{
    v3 direction = v3(-0.2f, 1.0f, -0.3f);

    glm::mat4 lightSpaceMatrix;
    glm::mat4 lightView;
    void updateView();
    
    void setLighting(ShaderModule* pShader);
    void DrawShadowMap(Renderer* pRender, ShaderModule* pShader) override;
    void BindShadowMap(ShaderModule* pLightingShader) override;
    FBO* pShadowFramebuffer = new FBO(FBO::FBO_SHADOWPASS);
    unsigned _type = eLightType::eDirectLight;
};

struct SpotLight
{
    v3 position = v3(0, 10, 0);
    v3 direction = v3(0, -1, 0);

    float constant = 1.f;
    float linear = 0.09f;
    float quadratic = 0.0032f;
    float cutOff = 12.5f;
    float outerCutOff = 15.f;

    float ambientStrength = 0.f;
    float diffuseStrength = 1.f;
    float farplane = SHADOW_CAST_FARPLANE;
    glm::mat4 lightSpaceMat;
    v3 color = v3(1.f);
    void setLighting(ShaderModule* pShader);
    void updateView();
    void DrawShadowMap(Renderer* pRender, ShaderModule* pShader);
    void BindShadowMap(ShaderModule* pLightingShader);
    FBO* pShadowFramebuffer = new FBO(FBO::FBO_SHADOWPASS);
};
struct PointLight
{

    v3 pos = v3(0.f, 5.f, 0.f);
    float ambient = 0.3f;

    v3 color = v3(1.f);
    float constant = 1.f;
    float linear = 0.009f;
    float quadratic = 0.032f;
    float farPlane = 1000.f;
    int idx = -1;
    void setLighting(ShaderModule* pShader);
    glm::mat4 shadowProj;
    glm::mat4 cubeMapFaceViews[6];
    glm::mat4 shadowTransforms[6] = // then for each face of the cubemap
    {
        glm::mat4(1.f),
        glm::mat4(1.f),
        glm::mat4(1.f),
        glm::mat4(1.f),
        glm::mat4(1.f),
        glm::mat4(1.f)
    };
    // Load the textures of each CM face into shader
    void setPersp(glm::mat4 persp, float far_Plane);
    
    void calcNewViews();
   
    void updateCubeFaces();
   
    void loadShadowCubeMapFaces(ShaderModule* pShader);
   

    void DrawShadowMap(Renderer* pRender, ShaderModule* pCubemapShader, ShaderModule* pLightingShader);
    void BindShadowMap(ShaderModule* pLightingShader);
    FBO* pShadowFramebuffer = new FBO(this, FBO::FBO_POINTLIGHT_SHADOWPASS);

};