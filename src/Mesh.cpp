#include "..\Header\Mesh.h"
#include"..\Header\Vertex.h"
#include"..\Header\Texture.h"
#include"..\Header\VAO.h"
#include"..\Header\VBO.h"
#include"..\Header\EBO.h"
#include"..\Header\ShaderModule.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures)
{
	_vertices = vertices;
	_indices = indices;
	_textures = textures;

	initialize();
}

void Mesh::initialize()
{
	_vao = new VAO();
	_vao->Bind();
	_vbo = new VBO(&_vertices[0], _vertices.size() * sizeof(Vertex));
	_ebo = new EBO(&_indices[0], _indices.size() * sizeof(unsigned int));
	_vbo->Bind();
	_ebo->Bind();
	//vert positions

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	_vao->Unbind();
	_vbo->Unbind();
	_ebo->Unbind();
}


void Mesh::Draw(ShaderModule* shader)
{
	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;
	for (unsigned int i = 0; i < _textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string num;
		std::string prefix;
		
		if (_textures[i]->_type == aiTextureType_DIFFUSE)
		{
			prefix = "material.texture_diffuse";
			num = std::to_string(diffuseNr++);
		}
		else if (_textures[i]->_type == aiTextureType_SPECULAR)
		{
			prefix = "material.texture_specular";
			num = std::to_string(specularNr++);
		}
		shader->setInt((prefix + num).c_str(), i);
		//_textures[i]->Bind();        
		glBindTexture(GL_TEXTURE_2D, _textures[i]->_id);


	}
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	_vao->Bind();
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	_vao->Unbind();

}
