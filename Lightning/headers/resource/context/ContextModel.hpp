#pragma once
#ifndef _CONTEXT_MODEL_H
#define _CONTEXT_MODEL_H

#include "resource/context/BaseContextResource.hpp"
#include "resource/context/ContextMesh.hpp"

namespace Resource
{
	namespace Context
	{
		class ContextModel : public Resource::Context::BaseContextResource
		{
		public:

			ContextModel(Manager::ContextResourceManager* container);
			~ContextModel();

			bool load(Manager::ResourceManager* resourceContainer, std::string path);
			
			char* type()
			{
				return "context_model";
			}

			std::vector<Resource::Context::ContextMesh> meshes; // List of meshes in this model

		private:
		};
	}
}

#endif