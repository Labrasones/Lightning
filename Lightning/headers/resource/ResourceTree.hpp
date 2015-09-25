/*
resource/ResourceTree.h

Contains a filetree storing a collection of resources
*/
#pragma once

#include <unordered_map>
#include <iostream>
#include <memory>
#include <utility>

#include "task/TaskList.hpp"
#include "ResourceNode.hpp"

namespace Manager
{
	class ResourceManager;
}

namespace Resource
{
	class BaseResourceTree
	{
	public:
		BaseResourceTree()
		{
		}
		virtual ~BaseResourceTree()
		{
		}
	};

	template <class ResourceType>
	class ResourceTree : public BaseResourceTree
	{
	// PUBLIC FUNCTIONS
	public:
		ResourceTree(Manager::ResourceManager *manager, std::string fallbackPath) : _manager(manager)
		{
			_root.name = "root"; // Rename the root node to 'root'
			_fallback.load(manager, fallbackPath); // Load the specified fallback resource
			ResourceNode<ResourceType>* fallbackNode = getNode(fallbackPath);
			fallbackNode->handle = std::shared_ptr<IntermediaryResourceHandle<ResourceType>>(&_fallback);
		}
		ResourceTree(Manager::ResourceManager *manager, ResourceType fallbackResource, std::string fallbackPath = "fallback") : _manager(manager)
		{
			_root.name = "root";
			_fallback = std::move(fallbackResource);
			getNode(fallbackPath);
		}
		~ResourceTree()
		{
			// Delete root's children
			for (auto it = _root.children.begin(); it != _root.children.end(); ++it)
			{
				deleteTreeAt(it->second);
			}
		}

		// Retrieve a resource from the tree. If the resource does not exist, a cloned fallback will be returned. If 'load' is true, resource will be added to the load queue
		ResourceNode<ResourceType>* getNode(std::string path)
		{
			std::cout << "Getting Resource" << std::endl;
			ResourceNode<ResourceType>* resourceNode = findPathNode(&_root, path);
			if (!resourceNode)
				throw std::out_of_range("Resource path not found, could not be added");
			return resourceNode;
		}

		void removeNode(std::string path)
		{
			ResourceNode<ResourceType>* resourceNode = findPathNode(&_root, path);
			if (resourceNode)
			{
				deleteTreeAt(resourceNode); // Delete it's children
				delete resourceNode; // Delete it
			}
		}

	// PRIVATE FUNCTIONS
	private:
		void deleteTreeAt(ResourceNode<ResourceType>* root){
			// Delete the tree rooted at each child
			for (auto it = root->children.begin(); it != root->children.end(); ++it)
			{
				deleteTreeAt(it->second);
			}
			// Delete this node
			delete root; // Delete the node
		}
		ResourceNode<ResourceType>* findPathNode(ResourceNode<ResourceType> *root, std::string path)
		{
			std::size_t pathBreak = path.find_first_of("/\\");
			if (pathBreak == std::string::npos) // All that is left of the path is the node name
			{
				std::cout << "End of the path" << std::endl;
				try // Try to get the node of this name
				{
					ResourceNode<ResourceType>* node = root->children.at(path);
					std::cout << "Path found: Returning" << std::endl;
					return node;
				}
				catch (std::out_of_range) // This named node was not in the tree under root
				{
					std::cout << "Path not found: Creating final node" << std::endl;
					ResourceNode<ResourceType>* child = new ResourceNode<ResourceType>(path, std::shared_ptr<ResourceHandle<ResourceType>>(new ResourceHandle<ResourceType>(&_fallback)));
					root->children[path] = child;
					return child;
				}
			}
			else{ // There is path remainaing
				std::string childName = path.substr(0, pathBreak);
				std::string remainaingPath = path.substr(pathBreak + 1);

				std::cout << "Finding: Looking for: " << childName << "; remaining: " << remainaingPath << std::endl;

				try{
					ResourceNode<ResourceType>* child = root->children.at(childName);
					std::cout << "Found child: " << childName << std::endl;
					return findPathNode(child, remainaingPath);
				}
				catch (std::out_of_range)
				{
					std::cout << "Child not found, creating it" << std::endl;
					ResourceNode<ResourceType>* child = new ResourceNode<ResourceType>(childName);
					root->children[childName] = child;
					return findPathNode(child, remainaingPath);
				}
			}
		}

		// CLASS MEMBERS
		private:
			Manager::ResourceManager *_manager; // Parent manager of this tree
			ResourceType _fallback; // fallback resource for this tree	
			ResourceNode<ResourceType> _root; // root of the resource tree
	};
}