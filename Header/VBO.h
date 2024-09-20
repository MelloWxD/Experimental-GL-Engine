#pragma once
#include"Constants.h"

class VBO
{
public:
	VBO(GLfloat* verts, GLsizeiptr size);
	~VBO();

	void Bind();
	void Unbind();
private:
	unsigned int _ID;
};

