#include "..\Header\Renderer.h"
#include "..\Header\Window.h"
#include "..\Header\ShaderModule.h"
#include "..\Header\AssetManager.h"
#include "..\Header\VAO.h"
#include "..\Header\VBO.h"
#include "..\Header\EBO.h"
#include "..\Header\Camera.h"
#include "..\Header\Texture.h"
#include "..\Header\Mesh.h"
#include "..\Header\Model.h"

Renderer::Renderer(Window* pWindow, AssetManager* pAM)
{
	assert(pWindow != nullptr);
	pWin = pWindow;
	pAssetManager = pAM;
	pCamera = new Camera();
	pCamera->position = v3(0.f, 0.f, 3.f);



	InitializeShaders();
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);


}

void Renderer::InitializeShaders()
{
	_pVAO = new VAO();
	_pVAO->Bind();
	_pVBO = new VBO(_vertices, sizeof(_vertices));
	_pEBO = new EBO(_indices, sizeof(_indices));
	pLightingShaderModule = new ShaderModule("shaders/basic_lighting_vert.glsl", "shaders/basic_lighting_frag.glsl"); // Lighting Shader
	pLightingCubeShaderModule = new ShaderModule("shaders/lightcube_vert.glsl", "shaders/lightcube_frag.glsl"); // LightCubeShader
	pModel = new Model("Assets/sponza.glb");

	_pVBO->Bind();
	

	_pVAO->LinkAttributes(*_pVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	
	_pVAO->LinkAttributes(*_pVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*) (3*sizeof(float)));

	
	_pVAO->LinkAttributes(*_pVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	_pVAO->Unbind();
	//_pVBO->Unbind();
	_pEBO->Unbind();

	_pLightVAO = new VAO();
	_pLightVAO->Bind();
	_pLightVBO = new VBO(lightVertices, sizeof(lightVertices));
	_pLightEBO = new EBO(lightIndices, sizeof(lightIndices));
	
	_pLightVAO->LinkAttributes(*_pVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	
	_pLightVAO->Unbind();
	_pLightVBO->Unbind();
	_pLightEBO->Unbind();
}


void Renderer::preRender()
{
	{
	//glm::mat4 model = glm::mat4(1.0f);
	////model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	////modelMat = model;

	//glm::mat4 view = glm::mat4(1.0f);
	//// note that we're translating the scene in the reverse direction of where we want to move
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//viewMat = view;

	//pCamera->Update(); // Update position

	//view = glm::lookAt(pCamera->position, pCamera->position + pCamera->Front, pCamera->Up);

	//glm::mat4 projection;
	//projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	//projMat = projection;

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pAssetManager->texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pAssetManager->texture2);*/




	//pShaderModule->setInt("texture1", 0); // or with shader class
	//pShaderModule->setInt("texture2", 1); // or with shader class

	//pShaderModule->setMat4("model", model);
	//pShaderModule->setMat4("view", view);
	//pShaderModule->setMat4("projection", projection);
	// 



	// render light cube
}
	
}
void Renderer::Render()
{
	glm::vec3 lightPos(1.2f, 10.0f, 2.0f);

	pLightingShaderModule->Use();

	pLightingShaderModule->setVec3("viewPos", pCamera->position);

	// Direct light
	pLightingShaderModule->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	pLightingShaderModule->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	pLightingShaderModule->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	pLightingShaderModule->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	pLightingShaderModule->setVec3("dirLight.color", 1.f, 1.f, 1.);


	// point lights
	pLightingShaderModule->setVec3("pointLights[0].position", pointLightPositions[0]);
	pLightingShaderModule->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	pLightingShaderModule->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	pLightingShaderModule->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	pLightingShaderModule->setVec3("pointLights[0].color", 1.0f, 1.0f, 1.0f);
	pLightingShaderModule->setFloat("pointLights[0].constant", 1.0f);
	pLightingShaderModule->setFloat("pointLights[0].linear", 0.09f);
	pLightingShaderModule->setFloat("pointLights[0].quadratic", 0.032f);
	// point light 2
	pLightingShaderModule->setVec3("pointLights[1].position", pointLightPositions[1]);
	pLightingShaderModule->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	pLightingShaderModule->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	pLightingShaderModule->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	pLightingShaderModule->setVec3("pointLights[1].color", 1.0f, 1.0f, 1.0f);
	pLightingShaderModule->setFloat("pointLights[1].constant", 1.0f);
	pLightingShaderModule->setFloat("pointLights[1].linear", 0.09f);
	pLightingShaderModule->setFloat("pointLights[1].quadratic", 0.032f);
	// point light 3
	pLightingShaderModule->setVec3("pointLights[2].position", pointLightPositions[2]);
	pLightingShaderModule->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	pLightingShaderModule->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	pLightingShaderModule->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	pLightingShaderModule->setVec3("pointLights[2].color", 1.0f, 1.0f, 1.0f);

	pLightingShaderModule->setFloat("pointLights[2].constant", 1.0f);
	pLightingShaderModule->setFloat("pointLights[2].linear", 0.09f);
	pLightingShaderModule->setFloat("pointLights[2].quadratic", 0.032f);
	// point light 4
	pLightingShaderModule->setVec3("pointLights[3].position", pointLightPositions[3]);
	pLightingShaderModule->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	pLightingShaderModule->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	pLightingShaderModule->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	pLightingShaderModule->setVec3("pointLights[3].color", 1.0f, 1.0f, 1.0f);

	pLightingShaderModule->setFloat("pointLights[3].constant", 1.0f);
	pLightingShaderModule->setFloat("pointLights[3].linear", 0.09f);
	pLightingShaderModule->setFloat("pointLights[3].quadratic", 0.032f);

	// spotLight
	pLightingShaderModule->setVec3("spotLight.position", pCamera->position);
	pLightingShaderModule->setVec3("spotLight.direction", pCamera->camFront);
	pLightingShaderModule->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	pLightingShaderModule->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	pLightingShaderModule->setVec3("spotLight.color", 1.0f, 1.0f, 0.0f);
	pLightingShaderModule->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	pLightingShaderModule->setFloat("spotLight.constant", 1.0f);
	pLightingShaderModule->setFloat("spotLight.linear", 0.09f);
	pLightingShaderModule->setFloat("spotLight.quadratic", 0.032f);
	pLightingShaderModule->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	pLightingShaderModule->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

	pLightingShaderModule->setFloat("material.shininess", 8);



	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(pCamera->_fov), (float)SCREEN_RES_X / (float)SCREEN_RES_Y, pCamera->nearPlane, pCamera->farPlane);
	pCamera->Update(); // Update position
	glm::mat4 view = glm::mat4(1.0f);

	pCamera->view = glm::lookAt(pCamera->position, pCamera->position + pCamera->Front, pCamera->camUp);
	pLightingShaderModule->setMat4("projection", projection);
	pLightingShaderModule->setMat4("view", pCamera->view);

	// world transformation
	glm::mat4 model = glm::mat4(1.0f);
	pLightingShaderModule->setMat4("model", model);

	pModel->Draw(pLightingShaderModule);

	

}

void Renderer::Display()
{
	preRender();
	//clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	Render();

	// Tell glfw to swap buffers to present new scene
	glfwSwapBuffers(pWin->window);

	
}
