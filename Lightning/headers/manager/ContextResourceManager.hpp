#pragma once
#ifndef CONTEXT_RESOURCE_MANAGER_H
#define CONTEXT_RESOURCE_MANAGER_H

#include "manager/ResourceManager.hpp"

namespace Manager {

	class ContextResourceManager
	{
	public:
		ContextResourceManager(Manager::ResourceManager *resourceManager) : _resources(resourceManager) {

		}
		~ContextResourceManager()
		{

		}

		template <class ContextResourceType>
		class ContextLoadTask : public Task::BasicTask
		{
		public:
			ContextLoadTask(Manager::ContextResourceManager* container,
							Manager::ResourceManager* resourceSource,
							Resource::ResourceNode<ContextResourceType>* resourceNode,
							std::string path,
							Task::priority_t priority = Task::CRITICAL) : BasicTask(priority),
																			_parentManager(container),
																			_nodeToLoad(resourceNode),
																			_resourceSource(resourceSource), 
																			_path(path)
			{}
			~ContextLoadTask()
			{}
			
			void execute()
			{
				// Load the resource
				Resource::ResourceHandle<ContextResourceType>* handle = _nodeToLoad->handle.get();
				ContextResourceType* loadedResource = new ContextResourceType(_parentManager);
				loadedResource->load(_manager, _path);
				if (loadedResource->loading() || loadedResource->loaded()) {
					handle->changeResource(loadedResource, false);
				}
			}
		private:
			Resource::ResourceNode<ContextResourceType>* _nodeToLoad;
			Manager::ResourceManager* _resourceSource;
			Manager::ContextResourceManager* _parentManager;
			std::string _path;
		};

		template <class ContextResourceType>
		void addLoadTask(Resource::ResourceNode<ContextResourceType> *resourceNode, std::string path, Task::priority_t pritority = Task::CRITICAL)
		{
			loadTaskList.addTask(new ContextLoadTask<ContextResourceType>(this, _resources, resourceNode, path, pritority));
		}
	private:
		Manager::ResourceManager* _resources;
	};
}

#endif