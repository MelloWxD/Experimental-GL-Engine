#pragma once
#include"Constants.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class Mesh;
class Texture;
class ShaderModule;

class Model
{
public:


	Model(const char* path)
	{
		loadFromFile(path);
	}

	void Draw(ShaderModule* shader);

private:
	void loadFromFile(std::string file_path);

	std::string directory;

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);

	std::vector<Mesh> _meshes; 
	std::vector<Texture*> _loaded_Textures;

};

