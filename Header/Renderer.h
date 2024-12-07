#pragma once
#include"Constants.h"
#include"ShaderModule.h"
#include"FBO.h"
#include"LightCasters.h"
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

    void drawObjects(ShaderModule* pShader, unsigned int DrawMode);
    void renderQuad();
    void drawSpotLightShadowMap();

    void RenderShadowCubeMap();
    void drawDirectionalShadowMap();
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

