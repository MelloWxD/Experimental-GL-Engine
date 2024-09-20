#pragma once
#include"Constants.h"

class EBO
{
public:
	EBO(GLuint* indices, GLsizeiptr size);
	~EBO();

	void Bind();
	void Unbind();

private:
	unsigned int _ID;
};