#include "../Header/RenderObject.h"
#include "../Header/ShaderModule.h"
#include "../Header/Model.h"
#include "../Header/Mesh.h"

RenderObject::RenderObject(Model* pm)
{
	pModel = pm;
	_name = "Unnamed";
	_mModelMat = m4(1.f);
	position = v3(0.f);
	rotation = glm::quat(0.f, 0.f, 0.f, 0.f);
	scale = v3(1.f);
}
RenderObject::RenderObject(v3 pos = v3(0), glm::quat rot = glm::quat(0.f,0.f,0.f,0.f), v3 scale = v3(1) )
{
	_name = "Unnamed";
	_mModelMat = m4(1.f);
	position = v3(0.f);
	rotation = glm::quat(0.f, 0.f, 0.f, 0.f);
	scale = v3(1.f);
}

void RenderObject::Update()
{
	_mModelMat = m4(1.f);
	_mModelMat = glm::translate(_mModelMat, position);
	_mModelMat = glm::rotate(_mModelMat, rotation.x, v3(1, 0, 0));
	_mModelMat = glm::rotate(_mModelMat, rotation.y, v3(0, 1, 0));
	_mModelMat = glm::rotate(_mModelMat, rotation.z, v3(0, 0, 1));
	_mModelMat = glm::scale(_mModelMat, scale);
}

void RenderObject::Draw(ShaderModule* pShader)
{
	pShader->Use();
	pShader->setMat4("model", _mModelMat);
	pModel->Draw(pShader);
}
