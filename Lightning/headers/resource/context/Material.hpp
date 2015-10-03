#pragma once
#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "resource/context/BaseContextResource.hpp"
#include "resource/context/Texture.hpp"

#include <glad/glad.h>

namespace Resource
{
	namespace Context
	{
		class Material : public BaseContextResource
		{
		public:
			Material();
			~Material();

			bool load(Manager::ResourceManager *resourceContainer, std::string path);
			char* type()
			{
				return "material";
			}
		private:
			std::vector<std::shared_ptr<Resource::Context::Texture>> _textures;
		};
	}
}

#endif _MATERIAL_H