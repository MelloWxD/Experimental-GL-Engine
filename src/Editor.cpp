
#define IMGUI_DEFINE_MATH_OPERATORS

#include "..\Header\Editor.h"
#include "..\Header\Window.h"
#include "..\Header\Renderer.h"
#include "..\Header\AssetManager.h"
#include "..\Header\Mesh.h"
#include "..\Header\Model.h"
#include "..\Header\RenderObject.h"
#include "..\Header\Camera.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imguizmo.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

float buff[4] = { 0.f, 0.f, 0.f, 0.f };


void dragVec3(const char* label, v3* v, float speed = 0.5f, float min = -FLT_MAX, float max = FLT_MAX, const char* fmt = "%.3f")
{
	buff[0] = v->x;
	buff[1] = v->y;
	buff[2] = v->z;
	ImGui::DragFloat3(label, buff, speed, min, max, fmt);
	v->x = buff[0];
	v->y = buff[1];
	v->z = buff[2];

}
void dragVec3(const char* label, glm::quat* q, float speed = 0.5f, float min = -FLT_MAX, float max = FLT_MAX, const char* fmt = "%.3f")
{
	buff[0] = glm::degrees(q->x);
	buff[1] = glm::degrees(q->y);
	buff[2] = glm::degrees(q->z);
	ImGui::DragFloat3(label, buff, speed, min, max, fmt);
	q->x = glm::radians(buff[0]);
	q->y = glm::radians(buff[1]);
	q->z = glm::radians(buff[2]);

}
void dragFloat(const char* label, float& f, float speed = 0.5f, float min = -FLT_MAX, float max = FLT_MAX, const char* fmt = "%.3f")
{
	buff[0] = f;
	ImGui::DragFloat(label, &f, speed, min, max, fmt);
	f = buff[0];
}
Editor::Editor(Window* pWin, AssetManager* pAM)
{
	_pWindow = pWin;
	pAssManager = pAM;
	init_imgui();
}

Editor::~Editor()
{
}

void Editor::Draw_Editor()
{
	if (!_enabled)
		return;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow();
	buff[0] = _pRenderer->pCamera->speed;
	buff[1] = _pRenderer->pCamera->_fov;
	// Camera Window
	{
		ImGui::Begin("Camera");
		ImGui::Text("Camera Position - %.3f, %.3f, %.3f", _pRenderer->pCamera->position.x, _pRenderer->pCamera->position.y, _pRenderer->pCamera->position.z);

		ImGui::DragFloat("Camera Speed", buff, 0.5f, -FLT_MAX, FLT_MAX, "%.3f");

		ImGui::DragFloat("Camera FOV", &buff[1], 0.5f, -FLT_MAX, FLT_MAX, "%.3f");

		_pRenderer->pCamera->speed = buff[0];
		_pRenderer->pCamera->_fov = buff[1];
		ImGui::End();
	}

	{
		ImGui::Begin("Lights");
		if (ImGui::Button("Toggle shadow view"))
		{
			_pRenderer->debug = !_pRenderer->debug;
		}
		if (ImGui::TreeNode("Directional Lighting"))
		{
			dragVec3("Direction", &_pRenderer->directionalLight.direction, 0.05f);
			dragVec3("Ambient", &_pRenderer->directionalLight.ambient, 0.005f);
			dragVec3("Diffuse", &_pRenderer->directionalLight.diffuse, 0.005f);
			dragVec3("Specular", &_pRenderer->directionalLight.specular, 0.005f);
			dragVec3("Colour", &_pRenderer->directionalLight.color, 0.005f);
			_pRenderer->directionalLight.setLighting(_pRenderer->pLightingShaderModule);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Torch Settings"))
		{
			auto& sl = _pRenderer->spotLight;
			dragVec3("Position [STATIC]", &sl.position, 0.05f);
			dragVec3("Direction [STATIC]", &sl.direction, 0.05f);
			dragVec3("Ambient", &sl.ambient, 0.005f);
			dragVec3("Diffuse", &sl.diffuse, 0.005f);
			dragFloat("cutOff", *&sl.cutOff, 0.005f);
			dragFloat("outerCutOff", *&sl.outerCutOff, 0.005f);
			dragVec3("Specular", &sl.specular, 0.005f);
			dragVec3("Colour", &sl.color, 0.005f);
			dragFloat("Constant", *&sl.constant, 0.005f);
			dragFloat("Linear", *&sl.linear, 0.005f);
			dragFloat("Quadratic", *&sl.quadratic, 0.005f);
			sl.setLighting(_pRenderer->pLightingShaderModule);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Point Lighting"))
		{
			for (int x = 0; x < _pRenderer->pointLights.size(); ++x)
			{
				if (ImGui::TreeNode(std::string("PointLight - " + std::to_string(x + 1)).c_str()))
				{
					auto& pl = _pRenderer->pointLights[x];
					dragVec3("Position", &pl.pos, 0.5f);
					dragVec3("Ambient", &pl.ambient, 0.005f);
					dragVec3("Diffuse", &pl.diffuse, 0.005f);
					dragVec3("Specular", &pl.specular, 0.005f);
					dragVec3("Colour", &pl.color, 0.005f);
					ImGui::Spacing();
					dragFloat("Constant", pl.constant, 0.005f);
					dragFloat("Linear", pl.linear, 0.005f);
					dragFloat("Quadratic", pl.quadratic, 0.005f);
					pl.setLighting(_pRenderer->pLightingShaderModule);
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
		ImGui::End();
	

	}
	// RenderObject viewer
	{
		ImGui::Begin("Testing Window");
		int cnt = 1;
		for (auto* r : _pRenderer->_vRenderObjects)
		{
			if (ImGui::TreeNode((std::to_string(cnt) + " - " + r->_name ).c_str()))
			{
				dragVec3("Position", &r->position, 0.05f);
				dragVec3("Rotation", &r->rotation, 0.05f);
				dragVec3("Scale", &r->scale, 0.05f);
				if (ImGui::TreeNode("Mesh Editor"))
				{
					for (auto& mesh : r->pModel->_meshes)
					{
						if (ImGui::TreeNode(mesh._name.c_str()))
						{
							if (ImGui::TreeNode("Texture Editor"))
							{
								ImGui::Text("Set Mesh Texture To");
								for (auto& tex : pAssManager->_texMap)
								{
									if (ImGui::Button(tex.first.c_str()))
									{
										mesh._textures.clear();
										mesh._textures.push_back(tex.second);
									}
								}




								ImGui::TreePop();

							}

							ImGui::TreePop();

						}
					}




					ImGui::TreePop();

				}

				if (ImGui::TreeNode("Model Editor"))
				{
					ImGui::Text("Set Model To");
					for (auto& mdl : pAssManager->_ModelMap)
					{
						if (ImGui::Button(mdl.first.c_str()))
						{
							
							r->pModel = mdl.second;
						}
					}




					ImGui::TreePop();

				}
				if (ImGui::TreeNode("Texture Editor"))
				{
					ImGui::Text("Set Texture To");
					for (auto& tex : pAssManager->_texMap)
					{
						if (ImGui::Button(tex.first.c_str()))
						{
							for (auto& mesh : r->pModel->_meshes)
							{
								mesh._textures.clear();
								mesh._textures.push_back(tex.second);

							}
						//	r->pModel->_loaded_Textures.clear();
						}
					}




					ImGui::TreePop();

				}
				ImGui::TreePop();
			}
			++cnt;
		}
		ImGui::End();
	}


	// Render ImGui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void Editor::ToggleActive()
{
	_enabled = !_enabled;
}

void Editor::init_imgui()
{	
	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfw_InitForOpenGL(_pWindow->window, true);
	ImGui_ImplOpenGL3_Init(OPENGL_VERSION);

}
