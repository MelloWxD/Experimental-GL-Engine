#pragma once
#include"Constants.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class ShaderModule;

struct Texture
{
	Texture(const char* img_path, GLuint slot, GLenum pixelType);
	Texture(const char* img_path, GLenum format, GLuint slot, GLenum pixelType);
	Texture(std::string img_path, GLuint slot, GLenum pixelType);
	Texture(const aiTexture* data_ptr, GLuint slot, GLenum pixelType, std::string path);
	Texture(int x, int y, GLuint slot, GLenum texture_type, GLenum filter_mode, unsigned clampMode);
	//Texture(const char* img_path, GLenum texture_type, GLuint slot, GLenum pixelType);
	~Texture();

	void texUnit(ShaderModule& shaderM, const char* uniform, GLuint unit);

	void Bind();
	void Unbind();
	unsigned int _id; // texture slot
	unsigned _type;
	GLuint _unit;
	std::string _path;
};

struct CubeMap
{
	CubeMap(std::string images_path);
	//CubeMap(const aiTexture* data_ptr, GLuint slot, GLenum pixelType, std::string path);
	//Texture(const char* img_path, GLenum texture_type, GLuint slot, GLenum pixelType);
	~CubeMap();

	void texUnit(ShaderModule& shaderM, const char* uniform, GLuint unit);

	std::vector<std::string> faces // dumb approach but who cares
	{
			"right.jpg",
			"left.jpg",
			"top.jpg",
			"bottom.jpg",
			"front.jpg",
			"back.jpg"
	};
	void Bind();
	void Unbind();
	unsigned int _id; // Cubemap texture slot
	aiTextureType _type;
	GLuint _unit;
	std::string _path;
};