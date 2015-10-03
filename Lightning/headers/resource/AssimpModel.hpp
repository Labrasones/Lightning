#pragma once
#ifndef _ASSIMP_MODEL_H
#define _ASSIMP_MODEL_H

#include "resource/Model.hpp"
#include "file/AssimpIO.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Resource
{
	class AssimpModel : public Model
	{
	public:
		AssimpModel();
		~AssimpModel();

		bool load(Manager::ResourceManager* container, std::string path);
		char* type() {
			return "assimp_model";
		}

	private:
		const aiScene* scene;
	};
}

#endif