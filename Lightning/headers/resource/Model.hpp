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
		Model() {};
		virtual ~Model() = 0;

		/* Resource Creation */
		bool load(Manager::ResourceManager* container, std::string path) = 0;
		char* type() {
			return "model";
		}
	};

	inline Model::~Model()
	{};
}

#endif