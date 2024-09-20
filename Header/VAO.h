#pragma once
#include"Constants.h"

class VBO;

class VAO
{
public:
	VAO();

	void LinkAttributes(VBO& vbo, unsigned int layout, unsigned int nComponents, GLenum type, GLsizeiptr stride, void* offset);


	void Bind();
	void Unbind();
	~VAO();

private:
	unsigned int _ID;
};