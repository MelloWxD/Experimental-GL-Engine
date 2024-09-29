#pragma once
#include"Constants.h"


struct Texture;
struct CubeMap;
struct Model;

class AssetManager
{
public:
	AssetManager();
	
	void loadModelFromPath(std::string file_path);
	void loadLooseTextures(std::string file_path);

	Texture* texture1;
	Texture* texture2;
	Texture* diffuseMap;
	Texture* specularMap;
	Texture* emissionMap;
	CubeMap* cubeMap;
	std::unordered_map<std::string, Model*> _ModelMap;
	std::unordered_map<std::string, Texture*> _texMap;
};

