#pragma once
#include"Constants.h"

class Model;
class ShaderModule;

class RenderObject
{
public:
	RenderObject(Model* pm);

	RenderObject(v3 pos, glm::quat rot, v3 scale);

	void Update();
	void Draw(ShaderModule* pShader, unsigned flag = 0u);
	std::string _name;

	v3 position;
	glm::quat rotation;
	v3 scale;
	bool _excludeFromShadowPass = false;
	glm::mat4 _mModelMat = glm::mat4(1);
	Model* pModel;
};

