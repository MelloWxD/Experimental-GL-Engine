#include "../Header/RenderObject.h"
#include "../Header/ShaderModule.h"
#include "../Header/Model.h"
#include "../Header/Mesh.h"

RenderObject::RenderObject(Model* pm)
{
	pModel = pm;
	_name = pm->name;
	_mModelMat = m4(1.f);
	position = v3(0.f);
	rotation = glm::quat(0.f, 0.f, 0.f, 0.f);
	scale = v3(1.f);
}
RenderObject::RenderObject(v3 pos = v3(0), glm::quat rot = glm::quat(0.f,0.f,0.f,0.f), v3 s = v3(1) )
{
	_name = "Unnamed";
	_mModelMat = m4(1.f);
	position = pos;
	rotation = rot;
	scale = s;
}

void RenderObject::Update()
{
	_mModelMat = m4(1.f);
	glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
	_mModelMat = glm::translate(_mModelMat, position);

	_mModelMat = glm::rotate(_mModelMat, rotation.x, v3(1.f, 0.f, 0.f));
	_mModelMat = glm::rotate(_mModelMat, rotation.y, v3(0.f, 1.f, 0.f));
	_mModelMat = glm::rotate(_mModelMat, rotation.z, v3(0.f, 0.f, 1.f));
	_mModelMat = glm::scale(_mModelMat, scale);
}

void RenderObject::Draw(ShaderModule* pShader, unsigned flag)
{
	if (_excludeFromShadowPass && flag)
	{
		return;
	}
	pShader->Use();
	pShader->setMat4("model", _mModelMat);
	pModel->Draw(pShader, flag);

}

