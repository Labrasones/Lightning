/*
	manager/ResourceManager.h

	Contains a list of ResourceTrees which hold references to various types of resources
*/
#pragma once

#include <unordered_map>
#include <string>
#include <typeinfo>
#include <typeindex>

#include "ResourceTree.hpp"
#include "ResourceHandle.hpp"

#include "WindowContext.hpp"
#include "FileManager.hpp"

namespace Manager
{
	class ResourceManager
	{
	public:
		ResourceManager(Context::WindowContext *context, File::FileManager *fileManager) : _context(context), _fileManager(fileManager)
		{

		}
		~ResourceManager()
		{

		}

		// Add a resource tree. Requires a path to a fallback resource. Replaces trees if that type already existed
		template <class ResourceType>
		void addTree(std::string fallbackPath)
		{
			Resource::ResourceTree<ResourceType>* tree = new Resource::ResourceTree<ResourceType>(this, fallbackPath);
			if (_trees.count(typeid(ResourceType)) > 0){
				delete _trees[typeid(ResourceType)]; // Destroy the tree at this location
			}
			_trees[typeid(ResourceType)] = tree;
		}
		template <class ResourceType>
		void createTreeOfType(ResourceType fallbackResource, std::string fallbackPath)
		{
			Resource::ResourceTree<ResourceType> *tree = new Resource::ResourceTree<ResourceType>(this, fallbackResource, fallbackPath);
			if (_trees.count(typeid(ResourceType)) > 0){
				delete _trees[typeid(ResourceType)];
			}
			_trees[typeid(ResourceType)] = tree;
		}

		// Get a resource tree.
		template <class ResourceType>
		Resource::ResourceTree<ResourceType>* getTree(){
			// Unique typeid of the ResourceType is used as the key. If the key exists, we are guarenteed that we can static_cast from BaseResourceTree to ResourceTree<ResourceType>
			Resource::BaseResourceTree* baseTree;

			try{
				baseTree = _trees.at(typeid(ResourceType)); // Get the resource tree
			}
			catch (std::out_of_range)
			{
				std::string s = "This type of resource does not have a tree";
				throw std::out_of_range(s);
			}
			Resource::ResourceTree<ResourceType>* tree = static_cast<Resource::ResourceTree<ResourceType>*>(baseTree); // Safe, uses typeid of ResourceType as key when retrieved. Either of type ResourceType or null.
			return tree;
		}

		// Get a resource from the tree. If load is true, will add it to the queue to be loaded. Supplying a tree will prevent tree lookups
		template <class ResourceType>
		Resource::ResourceHandle<ResourceType> getResourceHandle(std::string path, bool load = true, Resource::ResourceTree<ResourceType>* tree = nullptr){
			Resource::ResourceTree<ResourceType>* rtree = tree;
			if (!rtree)
			{
				rtree = getTree<ResourceType>(); // Get the resource tree
			}
			Resource::ResourceTree<ResourceType>::Node* resourceNode = rtree->getNode(path);

			// Return a resource handle containing a intermediary handle shared by the node.
			return Resource::ResourceHandle<ResourceType>(std::shared_ptr<Resource::IntermediaryResourceHandle<ResourceType>>(resourceNode->handle));
		}

		// Add a fully loaded resource to the tree for that type. Supplying a tree will prevent tree lookups
		template <class ResourceType>
		void addLoadedResource(std::string path, ResourceType* resource, Resource::ResourceTree<ResourceType>* tree = nullptr)
		{
			Resource::ResourceTree<ResourceType>* rtree = tree;
			if (!rtree)
			{
				rtree = getTree<ResourceType>();
			}
			Resource::ResourceTree<ResourceType>::Node* resourceNode = rtree->getNode(path);

			resourceNode->handle = std::shared_ptr<Resource::IntermediaryResourceHandle<ResourceType>
		}
		// Get the fileManager this resourceManager is using
		File::FileManager* file()
		{
			return _fileManager;
		}
		// Get the WindowContext this resourceManager is attached to
		Context::WindowContext* context()
		{
			return _context;
		}

		template <class ResourceType>
		class loadTask : public Task::BasicTask
		{
		public:
			loadTask(Manager::ResourceManager* parentManager, std::shared_ptr<ResourceType> resourceToLoad, std::string path, Task::priority_t priority = Task::HIGH) : BasicTask(priority), _resToLoad(resourceToLoad), _manager(parentManager), _path(path)
			{}
			~loadTask()
			{}
			void execute()
			{
				_resToLoad->load(_manager, _path);
			}
		private:
			std::shared_ptr<ResourceType> _resToLoad;
			Manager::ResourceManager* _manager;
			std::string _path;
		};

		template <class ResourceType>
		void addLoadTask(std::shared_ptr<ResourceType> resource, std::string path, Task::priority_t priority = Task::HIGH)
		{
			loadTaskList.addTask(new loadTask<ResourceType>(this, resource, path, priority));
		}

		Task::TaskList loadTaskList;

	private:
		// Local
		std::unordered_map<std::type_index, Resource::BaseResourceTree*> _trees;

		// Hierarchy links
		Context::WindowContext* _context;
		File::FileManager* _fileManager;
	};
}