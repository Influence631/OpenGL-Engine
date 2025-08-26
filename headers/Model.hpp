#pragma once
#include <Mesh.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <vector>
#include <Shader.hpp>

class Model{
	public:
	Model(const char* path);
	void Draw(Shader& shader);

	private:
	vector<Mesh> meshes;
	std::string directory;

	private:
	void loadModel(const char* path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);


};
