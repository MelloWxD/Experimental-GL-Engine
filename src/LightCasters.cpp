#include"..\Header\LightCasters.h"
#include"..\Header\Renderer.h"


void DirLight::updateView()
{
	// use ortho because directional lights are parallel
	float projSize = 50.f;
	glm::mat4 lightProjection = glm::ortho(-projSize, projSize, -projSize, projSize, 2.f, SHADOW_CAST_FARPLANE);

	/*lightView = glm::lookAt(direction * v3(10.f),
		v3(0.f),
		glm::vec3(1.0f, 1.0f, 1.0f));*/
	lightView = glm::lookAt(-direction * v3(10.f),
		v3(0.f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;

}

void DirLight::setLighting(ShaderModule* pShader)
{
	updateView();
	// Direct light	
	pShader->Use();

	pShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	pShader->setVec3("dirLight.direction", direction);
	pShader->setFloat("dirLight.ambientStrength", ambientStrength);
	pShader->setFloat("dirLight.diffuseStrength", diffuseStrength);
	pShader->setVec3("dirLight.color", color);
}

void DirLight::DrawShadowMap(Renderer* pRender, ShaderModule* pShader)
{
	setLighting(pShader);
	pShader->Use();
	pShadowFramebuffer->Bind();//glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	//RenderShadowCubeMap();
	pRender->Render(pShader, 1);
	pShadowFramebuffer->Unbind();
}

void DirLight::BindShadowMap(ShaderModule* pLightingShader)
{
	// Bind the shadow map
	glActiveTexture(GL_TEXTURE31);
	glBindTexture(GL_TEXTURE_2D, pShadowFramebuffer->_tex);
	//pShadowFramebuffer->_tex->Bind();
	pLightingShader->Use();
	pLightingShader->setInt("dirLight.shadowMap", 31);
	setLighting(pLightingShader); // Update and set light info in shader.
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

void PointLight::setPersp(glm::mat4 persp, float far_Plane)
{
	shadowProj = persp;
	farPlane = far_Plane;
}

void PointLight::calcNewViews()
{
	cubeMapFaceViews[0] = glm::lookAt(this->pos, this->pos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));  // +X
	cubeMapFaceViews[1] = glm::lookAt(this->pos, this->pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)); // -X
	cubeMapFaceViews[2] = glm::lookAt(this->pos, this->pos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));  // +Y (corrected up vector)
	cubeMapFaceViews[3] = glm::lookAt(this->pos, this->pos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));  // -Y (corrected up vector)
	cubeMapFaceViews[4] = glm::lookAt(this->pos, this->pos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));  // +Z
	cubeMapFaceViews[5] = glm::lookAt(this->pos, this->pos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)); // -Z
}

void PointLight::updateCubeFaces()
{
	calcNewViews();
	for (int i = 0; i < 6; ++i)
	{
		shadowTransforms[i] = shadowProj * cubeMapFaceViews[i];

	}

}

void PointLight::loadShadowCubeMapFaces(ShaderModule* pShader)
{
	updateCubeFaces();
	pShader->Use();
	//for (int x = 0; x < 6; ++x)
	//    {
	//        
	//        /*std::string s = "pointLights[" + std::to_string(idx);
	//        s += "].shadowMatrices[" +std::to_string(x) + "]";*/
	//    std::string s = "shadowMatrices[" + std::to_string(x);
	//    s += "]";
	//        pShader->setMat4(s, shadowProj * cubeMapFaceViews[x]);
	//    }
	glUniformMatrix4fv(glGetUniformLocation(pShader->ID, "shadowMatrices"), 6, GL_FALSE, glm::value_ptr(shadowTransforms[0]));

	pShader->setVec3("lightPos", pos);
	pShader->setFloat("farPlane", farPlane);
}

void PointLight::BindShadowMap(ShaderModule* pLightingShader)
{
	glActiveTexture(GL_TEXTURE29);
	glBindTexture(GL_TEXTURE_CUBE_MAP, pShadowFramebuffer->_depthCubemap);
	//pShadowFramebuffer->_tex->Bind();
	pLightingShader->Use();
	std::string entry = "pointLights[" + std::to_string(idx) + "].";

	pLightingShader->setInt(entry + "shadowCubeMap", 29);
	setLighting(pLightingShader); // Update and set light info in shader.

}

void PointLight::DrawShadowMap(Renderer* pRender, ShaderModule* pCubemapShader, ShaderModule* pLightingShader)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glm::mat4 lightProjection = glm::perspective(glm::radians(90.F), 1.f, 0.1f, SHADOW_CAST_FARPLANE); // Spot Light

	//pointLights[0].setPersp(lightProjection, SHADOW_CAST_FARPLANE);
	//pointLights[0].setLighting(pPointLightShadowCubemapShader);

	loadShadowCubeMapFaces(pCubemapShader);
	// render to point light fbo
	pShadowFramebuffer->Bind();//glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	pCubemapShader->Use();
	pCubemapShader->setVec3("lightPos", pos);
	pCubemapShader->setFloat("farPlane", SHADOW_CAST_FARPLANE);
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, -pos);
	pCubemapShader->setMat4("model", model);


	glGetError();
	glClear(GL_DEPTH_BUFFER_BIT);

	pRender->Render(pCubemapShader, 1); // 1 for DRAW_MODE_SHADOWPASS

	while (!pShadowFramebuffer->checkComplete())
	{
		printf("Framebuffer not complete");
	}
	pShadowFramebuffer->Unbind();//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE29);
	glBindTexture(GL_TEXTURE_CUBE_MAP, pShadowFramebuffer->_depthCubemap);
	pLightingShader->setFloat("farPlane", SHADOW_CAST_FARPLANE);
}

void SpotLight::setLighting(ShaderModule* pShader)
{
	updateView();
	pShader->Use();
	pShader->setVec3("spotLight.position", position);
	pShader->setVec3("spotLight.direction", direction);
	pShader->setFloat("spotLight.ambientStrength", ambientStrength);
	pShader->setFloat("spotLight.diffuse", diffuseStrength);
	pShader->setVec3("spotLight.color", color);
	pShader->setFloat("spotLight.constant", constant);
	pShader->setFloat("spotLight.linear", linear);
	pShader->setFloat("spotLight.quadratic", quadratic);
	pShader->setFloat("spotLight.cutOff", cos(glm::radians(cutOff)));
	pShader->setFloat("spotLight.outerCutOff", cos(glm::radians(outerCutOff)));
	pShader->setMat4("lightSpaceMatrix", lightSpaceMat);
	pShader->setFloat("farPlane", farplane);

}

void SpotLight::updateView()
{
	// direct light use orthogonal
	glm::mat4 lightProjection = glm::perspective(glm::radians(90.f), 1.f, 1.f, farplane);
	glm::mat4 lightView = glm::lookAt(position,
		position + glm::normalize(direction),
		glm::vec3(0.0f, 0.0f, -1.0f));
	lightSpaceMat = lightProjection * lightView;
}

void SpotLight::DrawShadowMap(Renderer* pRender, ShaderModule* pDepthShader)
{
	//spotLight.position = pCamera->position;
	//spotLight.direction = pCamera->Front;
	setLighting(pDepthShader);
	pDepthShader->Use();
	pShadowFramebuffer->Bind();//glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	//RenderShadowCubeMap();
	pRender->Render(pDepthShader, 1);
	pShadowFramebuffer->Unbind();
}
void SpotLight::BindShadowMap(ShaderModule* pLightingShader)
{
	glActiveTexture(GL_TEXTURE30);
	glBindTexture(GL_TEXTURE_2D, pShadowFramebuffer->_tex);
	//pShadowFramebuffer->_tex->Bind();
	pLightingShader->Use();
	pLightingShader->setInt("spotLight.shadowMap", 30);
	setLighting(pLightingShader); // Update and set light info in shader.
}