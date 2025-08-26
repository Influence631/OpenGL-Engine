#include <Mesh.hpp>
#include <vector>
#include <glad/glad.h>
#include <Shader.hpp>

using std::vector;

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) : vertices(vertices), indices(indices), textures(textures){	
	setupMesh();
}

void Mesh::setupMesh(){
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, UV)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Mesh::Draw(Shader& shader){
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for(unsigned int i = 0; i < this->textures.size(); i++){
		glActiveTexture(GL_TEXTURE0 + i); // active the appropriate texture unit before bidning the texture
		//retrieve the texture number (in the shader by convention we name them as texture_diffuseN / texture_specularN
		std::string number;
		std::string name = this->textures[i].type;
		
		if(name == "texture_diffuse")
			name += std::to_string(diffuseNr);
		else if (name == "texture_specular")
			name += std::to_string(specularNr);
		
		shader.setInt(("material." + name), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	//this is for consistancy, so the last unit is not activated after the function has been called.
	glActiveTexture(GL_TEXTURE0);

	//draw the mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
