#pragma once
#include <Mesh.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <vector>
#include <Shader.hpp>

using std::string;
using std::vector;

class Model{
	public:
	Model(const char* path);
	void Draw(Shader& shader);

	public:
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
  vector<Mesh>    meshes;
  string directory;
  bool gammaCorrection;
	
	private:
	void loadModel(const char* path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);


};
