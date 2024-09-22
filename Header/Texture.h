#pragma once
#include"Constants.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class ShaderModule;

struct Texture
{
	Texture(const char* img_path, GLuint slot, GLenum pixelType);
	Texture(const aiTexture* data_ptr, GLuint slot, GLenum pixelType, std::string path);
	//Texture(const char* img_path, GLenum texture_type, GLuint slot, GLenum pixelType);
	~Texture();

	void texUnit(ShaderModule& shaderM, const char* uniform, GLuint unit);

	void Bind();
	void Unbind();
	unsigned int _id; // texture slot
	aiTextureType _type;
	GLuint _unit;
	std::string _path;
};

