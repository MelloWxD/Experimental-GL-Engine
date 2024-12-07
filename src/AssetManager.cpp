#include "..\Header\AssetManager.h"
#include "..\Header\Texture.h"
#include "..\Header\Mesh.h"
#include "..\Header\Model.h"
#include <filesystem>

AssetManager::AssetManager()
{
	/*diffuseMap = new Texture("Assets/container2.png", GL_TEXTURE_2D, 0, GL_UNSIGNED_BYTE);
	specularMap = new Texture("Assets/container2_specular.png", GL_TEXTURE_2D, 1, GL_UNSIGNED_BYTE);
	emissionMap = new Texture("Assets/matrix.jpg", GL_TEXTURE_2D, 2, GL_UNSIGNED_BYTE);*/
	cubeMap = new CubeMap("Assets/cmSkybox");
}

void AssetManager::loadModelFromPath(std::string file_path)
{
	std::filesystem::path p = file_path;
	p.filename().filename();
	auto entryID = p.filename().stem().string();
	_ModelMap.emplace(std::pair<std::string, Model*>(entryID, new Model(file_path.c_str(), entryID)));
	
}

void AssetManager::loadLooseTextures(std::string file_path)
{
	std::filesystem::path p = file_path;
	int c = 0;
	for (auto& entry : std::filesystem::directory_iterator(p))
	{
		_texMap.emplace(std::pair<std::string, Texture*>(entry.path().stem().string(), new Texture(entry.path().string(), c, GL_UNSIGNED_BYTE)));
		++c;
	}
}

