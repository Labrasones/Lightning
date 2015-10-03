#pragma once
#ifndef CONTEXT_RESOURCE_H
#define CONTEXT_RESOURCE_H

#include "manager/ContextResourceManager.hpp"
#include "resource/BaseResource.hpp"

namespace Resource {
	namespace Context {
		class BaseContextResource : public BaseResource
		{
		public:
			BaseContextResource(Manager::ContextResourceManager* contextResourceManager) : BaseResource(), _contextResourceManager(contextResourceManager)
			{}
			virtual ~BaseContextResource() = 0;

			virtual bool load(Manager::ResourceManager *resourceContainer, std::string path) = 0;
		private:
			Manager::ContextResourceManager *_contextResourceManager;
		};

		inline BaseContextResource::~BaseContextResource()
		{

		}
	}
}

#endif