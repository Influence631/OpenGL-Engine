#include <Model.hpp>
#include <Mesh.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <vector>
#include <Shader.hpp>
#include <filesystem>

using std::vector;

Model::Model(const char* path){
	loadModel(path);
}

void Model::Draw(Shader& shader){
	for(int i = 0; i < this->meshes.size(); i++){
		this->meshes[i].Draw(shader);
	}
}

void Model::loadModel(const char* path){
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		std::cerr << "ERRORR ASSIMP:: "<<  import.GetErrorString(); << '\n';
		return;
	}
	std::filesystem::path filePath(path);
	this->directory = filePath.parent_path().string();
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene){
	//recursively process each of the node`s meshes
	for(int i = 0; i < node->mNumMeshes; i++){
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(processMesh(mesh, scene));
	}
	//do the same for all children
	for(int i = 0; i < node->mNumChildren; i++){
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene){
	//return 
	//need to finish the processMesh to read the assimp meshes into our custom Mesh type
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName){

}
