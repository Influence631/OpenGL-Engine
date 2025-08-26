#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Shader.hpp>

using std::vector;

struct Vertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 UV;
};

struct Texture{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh{
	public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;	

	public:
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	void Draw(Shader& shader);
	private:
	unsigned int VBO, EBO, VAO;
	private:

	void setupMesh();
};
