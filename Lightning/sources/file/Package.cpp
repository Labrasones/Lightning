/*
	core/fio/sources/Package.cpp

	Source file for the PackageDirectory object.
	Used to look up and retrieve data from within package objects.
*/

#include "file/Package.hpp"

#include <fstream>

using namespace File;
using namespace Source;

Package::Package()
{
	root = nullptr;
}

Package::~Package()
{
	// Delete the entire tree
	deleteTreeRootedAt(root);
	delete root; // Delete the highest level of the tree
	root = nullptr;
}

bool Package::openCatalog(std::string catalogFile)
{
	if (root != nullptr){ // There is already a tree present
		deleteTreeRootedAt(root);
		delete root;
		root = nullptr;
	}
	this->catalogFile = catalogFile;
	std::ifstream f(catalogFile);
	if (!f.is_open()){
		return false; // Couldn't open the package file, which means tree is null
	}
	// Parse the file and build the CatalogTree

	return true;
}

std::fstream Package::getFile(std::string filepath, std::ios::openmode mode){
	// find the filepath in the catalog tree
	// open the specified package and skip to the start of the file
	// copy from the package from the start byte to the end byte into a stringstream
	// create a new ifstream
	// set the ifstream's rdbuf to the stringstream
	// set stringstream's rdbuf to null
	// return ifstream

	// if the filepath wasn't found in the package, return nullptr
	return std::fstream();
}

void Package::deleteTreeRootedAt(File::Source::Package::CatalogNode* root)
{
	// for each child, if it has children, run deleteNodes with this child as root
	for (auto it = root->children.begin(); it != root->children.end(); ++it)
	{
		if (it->second->children.size() > 0){ // This is a node, delete it's subtree
			deleteTreeRootedAt(it->second);
		} // This node is now a leaf
		delete it->second; // Delete this leaf
		root->children.erase(it); // Remove it from the map
	}
}