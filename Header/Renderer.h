#pragma once
#include"Constants.h"
#include"ShaderModule.h"

class AssetManager;
class RenderObject;
class Editor;
class Window;
class Camera;
class Model;

// Buffer Object forward decs
class VBO;
class VAO;
class EBO;
class FBO;

struct DirLight
{
    v3 direction = v3(-0.2f, 1.0f, -0.3f);
    v3 ambient = v3(0.05f);
    v3 diffuse = v3(1);
    v3 specular = v3(1);
    v3 color = v3(1.f);
    void setLighting(ShaderModule* pShader);


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
    // spotLight
//pLightingShaderModule->setVec3("spotLight.position", spotLight.position);
//pLightingShaderModule->setVec3("spotLight.direction", spotLight.direction);
//////pLightingShaderModule->setVec3("spotLight.ambient", 0.1f, 0.1f, 0.1f);
//////pLightingShaderModule->setVec3("spotLight.diffuse", 0.1f, 0.1f, 0.1f);
//////pLightingShaderModule->setVec3("spotLight.color", 1.0f, 1.0f, 0.0f);
//////pLightingShaderModule->setVec3("spotLight.specular", 0.1f, 0.1f, 0.1f);
//pLightingShaderModule->setFloat("spotLight.constant", 1.0f);
//pLightingShaderModule->setFloat("spotLight.linear", 0.09f);
//pLightingShaderModule->setFloat("spotLight.quadratic", 0.032f);
//pLightingShaderModule->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
//pLightingShaderModule->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
    v3 ambient = v3(0);
    v3 diffuse = v3(1.f);
    v3 specular = v3(1.f);;
    v3 color = v3(1.f);;
    void setLighting(ShaderModule* pShader);

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
    void setPersp(const glm::mat4& persp, const float& far_Plane)
    {
        shadowProj = persp;
        farPlane = far_Plane;
       
        
    }
    void calcNewViews()
    {
        cubeMapFaceViews[0] = glm::lookAt(this->pos, this->pos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));  // +X
        cubeMapFaceViews[1] = glm::lookAt(this->pos, this->pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)); // -X
        cubeMapFaceViews[2] = glm::lookAt(this->pos, this->pos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)); // -Y
        cubeMapFaceViews[3] = glm::lookAt(this->pos, this->pos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));   // +Y
        cubeMapFaceViews[4] = glm::lookAt(this->pos, this->pos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));  // +Z
        cubeMapFaceViews[5] = glm::lookAt(this->pos, this->pos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)); // -Z
    }
    void updateCubeFaces()
    {
        calcNewViews();
        for (int i = 0; i < 6; ++i)
        {
            shadowTransforms[i] = shadowProj * cubeMapFaceViews[i];
            
        }
       
    }
    void loadShadowCubeMapFaces(ShaderModule* pShader)
    {
        updateCubeFaces();
        pShader->Use();
        for (int x = 0; x < 6; ++x)
            {
                
                /*std::string s = "pointLights[" + std::to_string(idx);
                s += "].shadowMatrices[" +std::to_string(x) + "]";*/
            std::string s = "shadowMatrices[" + std::to_string(x);
            s += "]";
                pShader->setMat4(s, shadowTransforms[x]);
            }
        pShader->setVec3("lightPos", pos);
        pShader->setFloat("farPlane", farPlane);
    }
    
};
class Renderer
{
public:
	Renderer(Window* pWin, AssetManager* pAM, Editor* pEDIT);

	/// <summary>
	/// Load buffers into shaders and that sort of thing
	/// </summary>
	void InitializeShaders();
	
    float skyboxVertices[108] = {
        // positions          
        -1000.f,  1000.f, -1000.f,
        -1000.f, -1000.f, -1000.f,
         1000.f, -1000.f, -1000.f,
         1000.f, -1000.f, -1000.f,
         1000.f,  1000.f, -1000.f,
        -1000.f,  1000.f, -1000.f,

        -1000.f, -1000.f,  1000.f,
        -1000.f, -1000.f, -1000.f,
        -1000.f,  1000.f, -1000.f,
        -1000.f,  1000.f, -1000.f,
        -1000.f,  1000.f,  1000.f,
        -1000.f, -1000.f,  1000.f,

         1000.f, -1000.f, -1000.f,
         1000.f, -1000.f,  1000.f,
         1000.f,  1000.f,  1000.f,
         1000.f,  1000.f,  1000.f,
         1000.f,  1000.f, -1000.f,
         1000.f, -1000.f, -1000.f,

        -1000.f, -1000.f,  1000.f,
        -1000.f,  1000.f,  1000.f,
         1000.f,  1000.f,  1000.f,
         1000.f,  1000.f,  1000.f,
         1000.f, -1000.f,  1000.f,
        -1000.f, -1000.f,  1000.f,

        -1000.f,  1000.f, -1000.f,
         1000.f,  1000.f, -1000.f,
         1000.f,  1000.f,  1000.f,
         1000.f,  1000.f,  1000.f,
        -1000.f,  1000.f,  1000.f,
        -1000.f,  1000.f, -1000.f,

        -1000.f, -1000.f, -1000.f,
        -1000.f, -1000.f,  1000.f,
         1000.f, -1000.f, -1000.f,
         1000.f, -1000.f, -1000.f,
        -1000.f, -1000.f,  1000.f,
         1000.f, -1000.f,  1000.f
    };
	void Render();

    void Render(ShaderModule* pShader, unsigned int DrawMode);

    void renderQuad();

    void RenderShadowCubeMap();

    void preRender();

	void Display();
    int debugID = 0;
    enum eDrawMode : unsigned int
    {
        DRAW_MODE_DEFAULT = 0,
        DRAW_MODE_SHADOWPASS = 1
    };
	m4 transformation_mat = m4(1.f);
	m4 viewMat = m4(1.f);
	m4 projMat;
	ShaderModule* pShaderModule;
	ShaderModule* pLightingShaderModule;

    
    float bias_low = 0.0001f;
    float bias_high = 0.001F;

    unsigned int quadVAO;
    unsigned int quadVBO;
	ShaderModule* pDepthShaderModule;
	ShaderModule* pDepthDefferedModule;
	ShaderModule* pPointLightShadowCubemapShader;

	ShaderModule* pSkyboxShaderModule;
	ShaderModule* pCubemapShaderModule;

    // Light structs
    std::vector<PointLight> pointLights;
    DirLight directionalLight;
    SpotLight spotLight;
    glm::vec3 pointLightPositions[4] = {
    glm::vec3(0.0f,  5.2f,  0.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
    };

    GLfloat lightVertices[24] =
    { //     COORDINATES     //
        -0.1f, -0.1f,  0.1f,
        -0.1f, -0.1f, -0.1f,
         0.1f, -0.1f, -0.1f,
         0.1f, -0.1f,  0.1f,
        -0.1f,  0.1f,  0.1f,
        -0.1f,  0.1f, -0.1f,
         0.1f,  0.1f, -0.1f,
         0.1f,  0.1f,  0.1f
    };

    GLuint lightIndices[36] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
    };
	// Render Object info
	float _vertices[288] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    bool debug = false;
    bool debug2 = false;
   
	unsigned int _indices[6] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	};
	m4 modelMat;

    Model* pModel;
    Model* pModel2;

	Window* pWin;
	AssetManager* pAssetManager;
	Camera* pCamera;
	Editor* _pEditorWindow;
	
    std::vector<RenderObject*> _vRenderObjects;


   /* unsigned int depthFBO;
    unsigned int depthMap;*/

    FBO* pShadowFramebuffer;
    FBO* pPointLightShadowFramebuffer;


    VAO* _pVAO;
    VBO* _pVBO;
    EBO* _pEBO;

    VAO* _pSkyboxVAO;
    VBO* _pSkyboxVBO;

    VAO* _pLightVAO;
    VBO* _pLightVBO;
    EBO* _pLightEBO;

	


};

