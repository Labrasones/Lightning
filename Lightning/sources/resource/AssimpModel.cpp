#include "resource/AssimpModel.hpp"

using namespace Resource;

AssimpModel::AssimpModel() 
{

}

AssimpModel::~AssimpModel()
{

}

bool AssimpModel::load(Manager::ResourceManager* container, std::string path)
{
	Assimp::Importer importer;
	importer.SetIOHandler(new File::AssimpIOSystem(container->file())); // Set the IO handler to a custom one to handle the sourced file system

	scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace); // Read a file and process it
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // Check if it failed to import
	{
		std::cout << "AssimpModel: Failed to load model..." << std::endl << importer.GetErrorString() << std::endl;
		_failed = true;
		return false;
	}

	// Process the scene into easily useable meshes
	return true;
}