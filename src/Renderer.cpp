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
	pDepthShaderModule = new ShaderModule("shaders/simpleDepth.vert", "shaders/emptyFrag.frag"); // simple depth write Shader
	pDepthDefferedModule = new ShaderModule("shaders/debugquad.vert", "shaders/debugquad.frag");
	pPointLightShadowCubemapShader = new ShaderModule("shaders/point_light_shadow_cubemap.vert", "shaders/point_light_shadow_cubemap.frag", "shaders/point_light_shadow_cubemap.geom"); // PointLight Shadowmap Shader
	//pLightingCubeShaderModule = new ShaderModule("shaders/lightcube_vert.glsl", "shaders/lightcube_frag.glsl"); // LightCubeShader
	//pModel = new Model("Assets/backpack/backpack.obj");
	//pModel2 = new Model("Assets/sponza-gltf-pbr/sponza.glb");
	//pAssetManager->loadModelFromPath("Assets/sponza-gltf-pbr/sponza.glb");
	pAssetManager->loadModelFromPath("Assets/cube.obj");
	pAssetManager->loadModelFromPath("Assets/sponza.glb");
	pAssetManager->loadModelFromPath("Assets/WaterBottle/WaterBottle.gltf");
	pAssetManager->loadModelFromPath("Assets/sphere.fbx");
	pAssetManager->loadLooseTextures("Assets/textures");
	pModel = pAssetManager->_ModelMap["cube"];
	pModel2 = pAssetManager->_ModelMap["sponza"];
	pModel2 = pAssetManager->_ModelMap["sphere"];
	//pModel2 = pAssetManager->_ModelMap["sponza"];
	_vRenderObjects.push_back(new RenderObject(pModel));
	_vRenderObjects.push_back(new RenderObject(pModel));
	_vRenderObjects.push_back(new RenderObject(pModel2));
	_vRenderObjects[_vRenderObjects.size() - 1]->scale = v3(.5f);

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
	pShadowFramebuffer = new FBO(FBO::FBO_SHADOWPASS); // Moved to classes
	pPointLightShadowFramebuffer = new FBO(&pointLights[0], FBO::FBO_POINTLIGHT_SHADOWPASS);
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

	//glm::mat4 lightProjection = glm::perspective(glm::radians(90.F), 1.f, 0.1f, SHADOW_CAST_FARPLANE); // Spot Light

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
	glActiveTexture(GL_TEXTURE30);
	glBindTexture(GL_TEXTURE_CUBE_MAP, pPointLightShadowFramebuffer->_depthCubemap);
	pLightingShaderModule->setInt("shadowCubeMap", 30);
	pLightingShaderModule->setFloat("farPlane", SHADOW_CAST_FARPLANE);
}
void Renderer::preRender()
{
	// shadow pass

	// view/projection transformations
	// Direct light
	
	directionalLight.setLighting(pLightingShaderModule);
	//spotLight.setLighting(pLightingShaderModule);
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
	
	


	/*pDepthDefferedModule->setMat4("model", model);
	pDepthDefferedModule->setMat4("projection", projection);
	pDepthDefferedModule->setMat4("view", directionalLight.lightView);*/

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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pShadowFramebuffer->_tex);
	pDepthDefferedModule->Use();
	pDepthDefferedModule->setInt("depthMap", 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
void Renderer::drawDirectionalShadowMap()
{
	directionalLight.setLighting(pDepthShaderModule); // Update and set light info in shader.
	pDepthShaderModule->Use();
	pShadowFramebuffer->Bind();//glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	//RenderShadowCubeMap();
	Render(pDepthShaderModule, 1);
	pShadowFramebuffer->Unbind();
}
void Renderer::drawSpotLightShadowMap()
{
	//spotLight.position = pCamera->position;
	//spotLight.direction = pCamera->Front;
	spotLight.setLighting(pDepthShaderModule);
	pDepthShaderModule->Use();
	pShadowFramebuffer->Bind();//glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	//RenderShadowCubeMap();
	Render(pDepthShaderModule, 1);
	pShadowFramebuffer->Unbind();
}
void Renderer::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	preRender();

	glDisable(GL_CULL_FACE);	

	//pointLights[0].setPersp(lightProjection, SHADOW_CAST_FARPLANE);
	//pointLights[0].setLighting(pPointLightShadowCubemapShader);

	
	

	// Resize viewport for shadow map drawing
	glViewport(0, 0, 4096, 4096);
	
	//pShadowFramebuffer->Bind();//glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//
	//Render(pDepthShaderModule, 1);
	//pShadowFramebuffer->Unbind();
	//drawDirectionalShadowMap();
	//drawSpotLightShadowMap();
	pointLights[0].DrawShadowMap(this, pPointLightShadowCubemapShader, pLightingShaderModule);


	/*directionalLight.DrawShadowMap(this, pDepthShaderModule);
	spotLight.DrawShadowMap(this, pDepthShaderModule);*/

	//RenderShadowCubeMap();
	// 2. RESET VIEWPORT then render scene as normal with shadow mapping (using depth map)
	glViewport(0, 0, SCREEN_RES_X, SCREEN_RES_Y);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);

	
	// Bind the shadow map
	//glActiveTexture(GL_TEXTURE31);
	//glBindTexture(GL_TEXTURE_2D, pShadowFramebuffer->_tex);
	////pShadowFramebuffer->_tex->Bind();
	//pLightingShaderModule->Use();
	//pLightingShaderModule->setInt("shadowSpotMap", 31);
	//spotLight.setLighting(pLightingShaderModule); // Update and set light info in shader.

	
	//directionalLight.BindShadowMap(pLightingShaderModule);
	spotLight.BindShadowMap(pLightingShaderModule);
	pointLights[0].BindShadowMap(pLightingShaderModule);



	//spotLight.position = pCamera->position;
	//spotLight.direction = pCamera->Front;
	//spotLight.setLighting(pLightingShaderModule); // Update and set light info in shader.

	//pLightingShaderModule->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	/*if (debug2)
	{
		pointLights[0].calcNewViews();
		pLightingShaderModule->setMat4("view", pointLights[0].cubeMapFaceViews[debugID]);
	}*/

	if (debug)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pShadowFramebuffer->_tex);
		// Use your debugging shader to visualize depth
		pDepthDefferedModule->Use();
		pDepthDefferedModule->setInt("shadowMap", 0);
		pDepthDefferedModule->setFloat("near_plane", 1.f);
		pDepthDefferedModule->setFloat("far_plane", SHADOW_CAST_FARPLANE);
		renderQuad();
	}
	else
	{
		
		Render(pLightingShaderModule);
	}
	_pEditorWindow->Draw_Editor();

	

	// Tell glfw to swap buffers to present new scene
	glfwSwapBuffers(pWin->window);

	
}
