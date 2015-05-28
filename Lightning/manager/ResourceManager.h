/*
	ResourceManager.h

	Handles loading unique resources to prevent duplication
*/
#pragma once

#include <memory>
#include <string>
#include <map>

#include "../resource/BaseResource.h"

namespace Resource
{
	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();

		std::shared_ptr<BaseResource> getResource(std::string file);
		void changeResource(std::string oldFile, std::string newFile);


	private:
		struct FileTreeNode {
			std::string fileName;
			std::shared_ptr<BaseResource> resource;
			bool isLeaf;
			std::map<std::string, FileTreeNode> children;
		};
	};
}