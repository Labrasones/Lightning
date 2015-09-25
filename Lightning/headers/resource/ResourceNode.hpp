/*
	ResourceNode
	A node in a resource tree
*/

#pragma once

#include <string>
#include <memory>

#include "ResourceHandle.hpp"

namespace Resource {
	template <class ResourceType>
	class ResourceNode { // A node in the resource tree
	public:
		// Create a new node with name, which points to resource (optional)
		ResourceNode(std::string name = "null", std::shared_ptr<ResourceHandle<ResourceType>> resourceHandle = std::shared_ptr<ResourceHandle<ResourceType>>()) : name(name), handle(resourceHandle), finished(false), loading(false)
		{}
		~ResourceNode()
		{}
		std::string name;
		std::shared_ptr<ResourceHandle<ResourceType>> handle;
		bool finished;
		bool loading;
		std::unordered_map<std::string, ResourceNode*> children;
	};
}