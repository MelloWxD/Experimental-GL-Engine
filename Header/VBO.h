#pragma once
#include"Constants.h"

class Vertex;
class VBO
{
public:
	VBO(GLfloat* verts, GLsizeiptr size);
	VBO(Vertex* verts, GLsizeiptr size);
	~VBO();

	void Bind();
	void Unbind();
private:
	unsigned int _ID;
};

