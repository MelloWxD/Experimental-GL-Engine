#include "..\Header\VBO.h"

VBO::VBO(GLfloat* verts, GLsizeiptr size)
{
	glGenBuffers(1, &_ID);
	glBindBuffer(GL_ARRAY_BUFFER, _ID);
	glBufferData(GL_ARRAY_BUFFER, size, verts, GL_STATIC_DRAW);
}

VBO::~VBO()
{
	glDeleteBuffers(1, &_ID);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, _ID);

}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}
