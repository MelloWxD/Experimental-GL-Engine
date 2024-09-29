#pragma once
#include"Constants.h"


#include "Vertex.h"
class Texture;
class ShaderModule;
class VAO;
class VBO;
class EBO;

class Mesh
{
public:

	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;
	std::vector<Texture*> _textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures, std::string name);
	void Draw(ShaderModule* shader);
	std::string _name;
private:
	//  render data
	VAO* _vao;
	VBO* _vbo;
	EBO* _ebo;
	void initialize();
};

