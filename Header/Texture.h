#pragma once
#include"Constants.h"

class ShaderModule;

struct Texture
{
	Texture(const char* imgData, GLenum texture_type, GLuint slot, GLenum format, GLenum pixelType);
	~Texture();

	void texUnit(ShaderModule& shaderM, const char* uniform, GLuint unit);

	void Bind();
	void Unbind();
	unsigned int _id; // texture slot
	GLenum _type;
	GLuint _unit;
};

