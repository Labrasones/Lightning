/*
	manager/ResourceManager.h

	Contains a list of ResourceTrees which hold references to various types of resources
*/
#pragma once

#include <unordered_map>
#include <string>
#include <typeinfo>
#include <typeindex>

#include "resource/ResourceTree.hpp"
#include "resource/ResourceHandle.hpp"
#include "resource/ResourceNode.hpp"

#include "FileManager.hpp"

namespace Manager
{
	class ResourceManager
	{
	public:
		ResourceManager(File::FileManager *fileManager) : _fileManager(fileManager)
		{

		}
		~ResourceManager()
		{
			_trees.clear(); // Delete all the trees in this manager
		}

		// Add a resource tree. Requires a path to a fallback resource. Replaces trees if that type already existed
		template <class ResourceType>
		void addTree(std::string fallbackPath)
		{
			ResourceType type; // instantiate an instance of the resource to get the typeid associated with the resource
			Resource::ResourceTree<ResourceType>* tree = new Resource::ResourceTree<ResourceType>(this, fallbackPath);
			if (_trees.count(type.getTypeID()) > 0){
				delete _trees[type.getTypeID()]; // Destroy the tree at this location
			}
			_trees[type.getTypeID()] = tree;
		}
		template <class ResourceType>
		void addTree(ResourceType fallbackResource, std::string fallbackPath)
		{
			ResourceType type; // instantiate an instance of the resource to get the typeid associated with the resource
			fallbackResource.makeUnready();
			Resource::ResourceTree<ResourceType> *tree = new Resource::ResourceTree<ResourceType>(this, fallbackResource, fallbackPath);
			if (_trees.count(type.getTypeID()) > 0){
				delete _trees[type.getTypeID()];
			}
			_trees[type.getTypeID()] = tree;
		}

		// Get a resource tree.
		template <class ResourceType>
		Resource::ResourceTree<ResourceType>* getTree(){
			ResourceType type; // instantiate an instance of the resource to get the typeid associated with the resource
			// Unique typeid of the ResourceType is used as the key. If the key exists, we are guarenteed that we can static_cast from BaseResourceTree to ResourceTree<ResourceType>
			Resource::BaseResourceTree* baseTree;
			try{
				baseTree = _trees.at(type.getTypeID()); // Get the resource tree
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
		std::shared_ptr<Resource::ResourceHandle<ResourceType>> getHandle(std::string path, bool load = true, Resource::ResourceTree<ResourceType>* tree = nullptr){
			Resource::ResourceTree<ResourceType>* rtree = tree;
			if (!rtree)
			{
				rtree = getTree<ResourceType>(); // Get the resource tree
			}
			Resource::ResourceNode<ResourceType> *node = rtree->getNode(path);

			if (!node->loading && !node->finished && load) { // If the node has not been loaded, is not currently loading, and we are set to load
				addLoadTask<ResourceType>(node, path);
				node->loading = true;
			}

			// Share the handle from the node
			return std::shared_ptr<Resource::ResourceHandle<ResourceType>>(node->handle);
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
			Resource::ResourceNode<ResourceType>* resourceNode = rtree->getNode(path);

			resourceNode->handle = std::shared_ptr<Resource::ResourceHandle<ResourceType>>(new Resource::ResourceHandle<ResourceType>(resource));
		}
		// Get the fileManager this resourceManager is using
		File::FileManager* file()
		{
			return _fileManager;
		}

		template <class ResourceType>
		class loadTask : public Task::BasicTask
		{
		public:
			loadTask(Manager::ResourceManager* parentManager, Resource::ResourceNode<ResourceType> *resourceNode, std::string path, Task::priority_t priority = Task::HIGH) : BasicTask(priority), _nodeToLoad(resourceNode), _manager(parentManager), _path(path)
			{}
			~loadTask()
			{}
			void execute()
			{
				// Load the resource
				Resource::ResourceHandle<ResourceType>* handle = _nodeToLoad->handle.get();
				ResourceType* loadedResource = new ResourceType();
				loadedResource->load(_manager, _path);
				if (loadedResource->loading() || loadedResource->loaded()) {
					handle->changeResource(loadedResource, false);
				}
			}
		private:
			Resource::ResourceNode<ResourceType>* _nodeToLoad;
			Manager::ResourceManager* _manager;
			std::string _path;
		};

		template <class ResourceType>
		void addLoadTask(Resource::ResourceNode<ResourceType> *resourceNode, std::string path, Task::priority_t priority = Task::HIGH)
		{
			loadTaskList.addTask(new loadTask<ResourceType>(this, resourceNode, path, priority));
		}

		Task::TaskList loadTaskList;

	private:
		// Local
		std::unordered_map<std::type_index, Resource::BaseResourceTree*> _trees;

		// Hierarchy links
		File::FileManager* _fileManager;
	};
}