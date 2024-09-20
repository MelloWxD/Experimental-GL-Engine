#include"..\Header\VAO.h"
#include"..\Header\VBO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &_ID);
}

void VAO::LinkAttributes(VBO& vbo, unsigned int layout_location, unsigned int nComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	vbo.Bind();
	glVertexAttribPointer(layout_location, nComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout_location);
	vbo.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(_ID);

}

void VAO::Unbind()
{
	glBindVertexArray(0);

}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &_ID);
}
