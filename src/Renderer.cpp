#include "..\Header\Renderer.h"
#include "..\Header\Window.h"
#include "..\Header\ShaderModule.h"
#include "..\Header\AssetManager.h"
#include "..\Header\Editor.h"
#include "..\Header\VAO.h"
#include "..\Header\VBO.h"
#include "..\Header\EBO.h"
#include "..\Header\FBO.h"
#include "..\Header\Camera.h"
#include "..\Header\Texture.h"
#include "..\Header\Mesh.h"
#include "..\Header\Model.h"
#include "..\Header\RenderObject.h"




Renderer::Renderer(Window* pWindow, AssetManager* pAM, Editor* pEDIT)
{
	assert(pWindow != nullptr);
	pWin = pWindow;
	pAssetManager = pAM;
	pCamera = new Camera();
	pCamera->position = v3(0.f, 0.f, 3.f);
	_pEditorWindow = pEDIT;


	InitializeShaders();
	
	

	
}

void Renderer::InitializeShaders()
{
	_pVAO = new VAO();
	_pVAO->Bind();
	_pVBO = new VBO(_vertices, sizeof(_vertices));
	_pEBO = new EBO(_indices, sizeof(_indices));
	//pSkyboxShaderModule = new ShaderModule("shaders/skybox.vert", "shaders/skybox.frag"); // skybox Shader
	//pCubemapShaderModule = new ShaderModule("shaders/cmBasic.vert", "shaders/cmBasic.frag"); // cubemap Shader
	pLightingShaderModule = new ShaderModule("shaders/basic_lighting_Shadow.vert", "shaders/basic_lighting_shadow.frag"); // Lighting Shader
	pDepthShaderModule = new ShaderModule("shaders/simpleDepth.vert", "shaders/emptyFrag.frag"); // Depthmap Shader

	pPointLightShadowCubemapShader = new ShaderModule("shaders/point_light_shadow_cubemap.vert", "shaders/point_light_shadow_cubemap.frag", "shaders/point_light_shadow_cubemap.geom"); // PointLight Shadowmap Shader
	//pLightingCubeShaderModule = new ShaderModule("shaders/lightcube_vert.glsl", "shaders/lightcube_frag.glsl"); // LightCubeShader
	//pModel = new Model("Assets/backpack/backpack.obj");
	//pModel2 = new Model("Assets/sponza-gltf-pbr/sponza.glb");
	//pAssetManager->loadModelFromPath("Assets/sponza-gltf-pbr/sponza.glb");
	pAssetManager->loadModelFromPath("Assets/cube.obj");
	pAssetManager->loadModelFromPath("Assets/sphere.fbx");
	pAssetManager->loadLooseTextures("Assets/textures");
	pModel = pAssetManager->_ModelMap["cube"];
	pModel2 = pAssetManager->_ModelMap["cube"];
	//pModel2 = pAssetManager->_ModelMap["sponza"];
	_vRenderObjects.push_back(new RenderObject(pModel));
	_vRenderObjects.push_back(new RenderObject(pModel));
	_vRenderObjects.push_back(new RenderObject(pModel2));

	_vRenderObjects.push_back(new RenderObject(pModel));
	_vRenderObjects[_vRenderObjects.size() - 1]->position = v3(5, 5, 0);
	_vRenderObjects.push_back(new RenderObject(pModel));
	_vRenderObjects[_vRenderObjects.size() - 1]->position = v3(-5, 5, 0);

	_vRenderObjects.push_back(new RenderObject(pModel));
	_vRenderObjects[_vRenderObjects.size() - 1]->position = v3(0, 5, 5);

	_vRenderObjects.push_back(new RenderObject(pModel));
	_vRenderObjects[_vRenderObjects.size() - 1]->position = v3(0, 5, -5);

	_vRenderObjects.push_back(new RenderObject(pModel));
	_vRenderObjects[_vRenderObjects.size() - 1]->position = v3(0, 0, 0);

	_vRenderObjects.push_back(new RenderObject(pModel));
	_vRenderObjects[_vRenderObjects.size() - 1]->position = v3(0, 10, 0);




	_vRenderObjects[1]->position = v3(0, -15.f, 0);
	_vRenderObjects[1]->scale = v3(1000, 1, 1000);
	_vRenderObjects[1]->_excludeFromShadowPass = true;
	for (int x = 0; x < 4; ++x)
	{
		PointLight pl;
		pl.pos = pointLightPositions[x];
		pl.idx = x;
		pointLights.push_back(pl);
	}
	pointLights[0].pos = v3(0, 5, 0);
	//create Frame Buffer
	//glGenFramebuffers(1, &depthFBO);

	//// Create texture for the depth map to write to
	//const unsigned int W = 1024, H = 1024;
	//glGenTextures(1, &depthMap);
	//glBindTexture(GL_TEXTURE_2D, depthMap);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
	//	W, H, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	////// attach texture to the framebuffer
	//glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	pShadowFramebuffer = new FBO(FBO::FBO_SHADOWPASS);
	pPointLightShadowFramebuffer = new FBO(pLightingShaderModule, &pointLights[0], FBO::FBO_POINTLIGHT_SHADOWPASS);
	/*pDepthDefferedModule->Use();
	pDepthDefferedModule->setInt("depthMap", 31);*/
	/*_pSkyboxVAO = new VAO();
	_pSkyboxVAO->Bind();
	_pSkyboxVBO = new VBO(skyboxVertices, sizeof(skyboxVertices));
	_pSkyboxVBO->Bind();
	_pSkyboxVAO->LinkAttributes(*_pSkyboxVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	_pSkyboxVAO->Unbind();
	_pSkyboxVBO->Unbind();*/
	int c = 0;
	for (auto& pl : pointLights)
	{
		pl.pos = pointLightPositions[c];
		++c;
	}

	// Point Lights shadow pass
	float near_plane = 0.1f, far_plane = 10000.f;;// 0.1f, 
	// direct light use orthogonal
	//glm::mat4 lightProjection = glm::ortho(-35.F, 35.f, -35.f, 35.f, -100.f, 100.f); // Dir Light
	glm::mat4 lightProjection = glm::perspective(glm::radians(90.F), 1.f, near_plane, far_plane); // Spot Light
	glm::mat4 lightView = glm::lookAt(spotLight.position,
		spotLight.position + glm::normalize(spotLight.direction),
		glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	

}

void Renderer::RenderShadowCubeMap()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glm::mat4 lightProjection = glm::perspective(glm::radians(90.F), 1.f, 0.1f, SHADOW_CAST_FARPLANE); // Spot Light

	//pointLights[0].setPersp(lightProjection, SHADOW_CAST_FARPLANE);
	//pointLights[0].setLighting(pPointLightShadowCubemapShader);
	pointLights[0].loadShadowCubeMapFaces(pPointLightShadowCubemapShader);
	// render to point light fbo
	pPointLightShadowFramebuffer->Bind();//glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	pPointLightShadowCubemapShader->Use();
	pPointLightShadowCubemapShader->setVec3("lightPos", pointLights[0].pos);
	pPointLightShadowCubemapShader->setFloat("farPlane", SHADOW_CAST_FARPLANE);
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, -pointLights[0].pos);
	pPointLightShadowCubemapShader->setMat4("model", model);

	
	glGetError();
	glClear(GL_DEPTH_BUFFER_BIT);

	Render(pPointLightShadowCubemapShader, DRAW_MODE_SHADOWPASS);
	
	while (!pPointLightShadowFramebuffer->checkComplete())
	{
		printf("Framebuffer not complete");
	}
	pPointLightShadowFramebuffer->Unbind();//glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void Renderer::preRender()
{
	// shadow pass

	directionalLight.setLighting(pLightingShaderModule);
	// view/projection transformations
	// Direct light
	//directionalLight.setLighting(pLightingShaderModule);
	
	//// point lights
	//pLightingShaderModule->setVec3("pointLights[0].position", pointLightPositions[0]);
	//pLightingShaderModule->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	//pLightingShaderModule->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	//pLightingShaderModule->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	//pLightingShaderModule->setVec3("pointLights[0].color", 1.0f, 1.0f, 1.0f);
	//pLightingShaderModule->setFloat("pointLights[0].constant", 1.0f);
	//pLightingShaderModule->setFloat("pointLights[0].linear", 0.09f);
	//pLightingShaderModule->setFloat("pointLights[0].quadratic", 0.032f);
	//// point light 2
	//pLightingShaderModule->setVec3("pointLights[1].position", pointLightPositions[1]);
	//pLightingShaderModule->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	//pLightingShaderModule->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	//pLightingShaderModule->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	//pLightingShaderModule->setVec3("pointLights[1].color", 1.0f, 0.0f, 1.0f);
	//pLightingShaderModule->setFloat("pointLights[1].constant", 1.0f);
	//pLightingShaderModule->setFloat("pointLights[1].linear", 0.09f);
	//pLightingShaderModule->setFloat("pointLights[1].quadratic", 0.032f);
	//// point light 3
	//pLightingShaderModule->setVec3("pointLights[2].position", pointLightPositions[2]);
	//pLightingShaderModule->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	//pLightingShaderModule->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	//pLightingShaderModule->setVec3("pointLights[2].specular", 0.0f, 1.0f, 1.0f);
	//pLightingShaderModule->setVec3("pointLights[2].color", 1.0f, 1.0f, 1.0f);

	//pLightingShaderModule->setFloat("pointLights[2].constant", 1.0f);
	//pLightingShaderModule->setFloat("pointLights[2].linear", 0.09f);
	//pLightingShaderModule->setFloat("pointLights[2].quadratic", 0.032f);
	//// point light 4
	//pLightingShaderModule->setVec3("pointLights[3].position", pointLightPositions[3]);
	//pLightingShaderModule->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	//pLightingShaderModule->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	//pLightingShaderModule->setVec3("pointLights[3].specular", 1.0f, 1.0f, 0.0f);
	//pLightingShaderModule->setVec3("pointLights[3].color", 1.0f, 1.0f, 1.0f);

	//pLightingShaderModule->setFloat("pointLights[3].constant", 1.0f);
	//pLightingShaderModule->setFloat("pointLights[3].linear", 0.09f);
	//pLightingShaderModule->setFloat("pointLights[3].quadratic", 0.032f);
	
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
	spotLight.setLighting(pLightingShaderModule);
	pLightingShaderModule->Use();
	pLightingShaderModule->setFloat("material.shininess", 8);
	pLightingShaderModule->setVec3("viewPos", pCamera->position);
	pLightingShaderModule->setVec3("viewDir", pCamera->Front);


	glm::mat4 projection = glm::perspective(glm::radians(pCamera->_fov), (float)SCREEN_RES_X / (float)SCREEN_RES_Y, pCamera->nearPlane, pCamera->farPlane);
	pCamera->Update(); // Update position
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	pLightingShaderModule->setMat4("projection", projection);
	pCamera->view = glm::lookAt(pCamera->position, pCamera->position + pCamera->Front, pCamera->camUp);
	pLightingShaderModule->setMat4("view", pCamera->view);
	pLightingShaderModule->setMat4("model", model);

	pLightingShaderModule->setFloat("biasLow", bias_low);
	pLightingShaderModule->setFloat("biasHigh", bias_high);


	//pLightingShaderModule->setMat4("model", model);
	//pLightingShaderModule->setMat4("projection", projection);
	//pLightingShaderModule->setMat4("view", pCamera->view);
	// world transformation
	//pLightingShaderModule->setMat4("model", model);
	//pModel->Draw(pCubemapShaderModule);

	//_vRenderObjects[2]->position = directionalLight.direction;

	

}
	

void Renderer::Render()
{

	
	
	for (auto r : _vRenderObjects)
	{
		r->Update();
		r->Draw(pLightingShaderModule);
	}

	// glDepthFunc(GL_LEQUAL);
	//pSkyboxShaderModule->Use();
	//pSkyboxShaderModule->setMat4("model", model);
	//pSkyboxShaderModule->setMat4("projection", projection);
	//pSkyboxShaderModule->setMat4("view", pCamera->view);

	//_pSkyboxVAO->Bind();
	//pAssetManager->cubeMap->Bind();
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//_pSkyboxVAO->Unbind();

	//glDepthMask(GL_TRUE);
	//
	

	

}
void Renderer::Render(ShaderModule* pShader, unsigned int DrawMode = DRAW_MODE_DEFAULT)
{
	
	for (auto r : _vRenderObjects)
	{
		r->Update();
		r->Draw(pShader, DrawMode);
	}
	

	// glDepthFunc(GL_LEQUAL);
	//pSkyboxShaderModule->Use();
	//pSkyboxShaderModule->setMat4("model", model);
	//pSkyboxShaderModule->setMat4("projection", projection);
	//pSkyboxShaderModule->setMat4("view", pCamera->view);

	//_pSkyboxVAO->Bind();
	//pAssetManager->cubeMap->Bind();
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//_pSkyboxVAO->Unbind();

	//glDepthMask(GL_TRUE);
	//
	

	

}
void Renderer::renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
void Renderer::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	//spotLight.position = pCamera->position;
	//float near_plane = 2.f, far_plane = 100.5f;
	//// use ortho because directional lights are 'parallel'

	//glm::mat4 lightProjection = glm::ortho(-35.F, 35.f, -35.f, 35.f, near_plane, far_plane);
	//
	//glm::mat4 lightView = glm::lookAt(directionalLight.direction,
	//	v3(0.f),
	//	glm::vec3(1.0f, 1.0f, 1.0f));	

	//float near_plane = 0.1f, far_plane = 10000.f;;// 0.1f, 
	//// direct light use orthogonal
	////glm::mat4 lightProjection = glm::ortho(-35.F, 35.f, -35.f, 35.f, -100.f, 100.f); // Dir Light
	//glm::mat4 lightView = glm::lookAt(spotLight.position,
	//	spotLight.position + glm::normalize(spotLight.direction),
	//	glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	//pointLights[0].setPersp(lightProjection, SHADOW_CAST_FARPLANE);
	//pointLights[0].setLighting(pPointLightShadowCubemapShader);

	
	/*pDepthShaderModule->Use();
	pDepthShaderModule->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	pDepthShaderModule->setMat4("proj", lightProjection);
	pDepthShaderModule->setMat4("view", lightView);*/
	//pDepthShaderModule->setMat4("model", glm::mat4(1.f));

	glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);

	// Spot light shadow framebuffer
	glViewport(0, 0, 4096, 4096);
	//pShadowFramebuffer->Bind();//glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	RenderShadowCubeMap();

	glClear(GL_DEPTH_BUFFER_BIT);
	//Render(pDepthShaderModule); 
	//glGetError();
	//while (!pShadowFramebuffer->checkComplete())
	//{
	//	printf("Framebuffe not complete");
	//}
	//pShadowFramebuffer->Unbind();//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	// 2. RESET VIEWPORT then render scene as normal with shadow mapping (using depth map)
	glViewport(0, 0, SCREEN_RES_X, SCREEN_RES_Y);
	glCullFace(GL_BACK);

	preRender();
	
	// Bind the shadow map
	glActiveTexture(GL_TEXTURE31);
	glBindTexture(GL_TEXTURE_CUBE_MAP, pPointLightShadowFramebuffer->_depthCubemap);
	//pShadowFramebuffer->_tex->Bind();
	//pLightingShaderModule->setInt("shadowMap", 31);
	pLightingShaderModule->setInt("shadowCubeMap", 31);
	pLightingShaderModule->setFloat("farPlane", SHADOW_CAST_FARPLANE);
	//pLightingShaderModule->setMat4("lightSpaceMatrix", lightSpaceMatrix);

	if (debug2)
	{
		pointLights[0].calcNewViews();
		pLightingShaderModule->setMat4("view", pointLights[0].cubeMapFaceViews[debugID]);
	}

	if (debug)
	{


	
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, pShadowFramebuffer->_depthCubemap);*/
		//pShadowFramebuffer->_tex->Bind();
		//renderQuad();
	}

	Render(pLightingShaderModule);

	_pEditorWindow->Draw_Editor();

	

	// Tell glfw to swap buffers to present new scene
	glfwSwapBuffers(pWin->window);

	
}

void DirLight::setLighting(ShaderModule* pShader)
{  
	// Direct light	
	pShader->Use();

	pShader->setVec3("dirLight.direction", direction);
	pShader->setVec3("dirLight.ambient", ambient);
	pShader->setVec3("dirLight.diffuse", diffuse);
	pShader->setVec3("dirLight.specular", specular);
	pShader->setVec3("dirLight.color", color);
}

void PointLight::setLighting(ShaderModule* pShader)
{
	if (idx < 0)
	{
		assert(false);
	}
	std::string entry = "pointLights[" + std::to_string(idx) + "].";
	pShader->setVec3(std::string(entry + "Position").c_str(), pos);
	pShader->setFloat(std::string(entry + "ambientStrength").c_str(), ambient);

	pShader->setVec3(std::string(entry + "color").c_str(), color);
	pShader->setFloat(std::string(entry + "constant").c_str(), constant);
	pShader->setFloat(std::string(entry + "linear").c_str(), linear);
	pShader->setFloat(std::string(entry + "quadratic").c_str(), quadratic);
}

void SpotLight::setLighting(ShaderModule* pShader)
{
	pShader->Use();
	pShader->setVec3("spotLight.position", position);
	pShader->setVec3("spotLight.direction", direction);
	pShader->setVec3("spotLight.ambient", ambient);
	pShader->setVec3("spotLight.diffuse", diffuse);
	pShader->setVec3("spotLight.color", color);
	pShader->setVec3("spotLight.specular", specular);
	pShader->setFloat("spotLight.constant", constant);
	pShader->setFloat("spotLight.linear", linear);
	pShader->setFloat("spotLight.quadratic", quadratic);
	pShader->setFloat("spotLight.cutOff", cos(glm::radians(cutOff)));
	pShader->setFloat("spotLight.outerCutOff", cos(glm::radians(outerCutOff)));
}
