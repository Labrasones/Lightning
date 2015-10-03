#pragma once
#ifndef _MODEL_H
#define _MODEL_H

#include "BaseResource.hpp"
#include "Mesh.hpp"

#include <vector>

namespace Resource
{
	class Model : public BaseResource
	{
	public:
		Model();
		~Model();

		/* Resource Creation */
		bool load(Manager::ResourceManager* container, std::string path);
		char* type() {
			return "model";
		}
	};
}

#endif