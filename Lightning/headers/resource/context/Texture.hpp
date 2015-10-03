#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "resource/context/BaseContextResource.hpp"
#include "resource/Image.hpp"

#include <glad/glad.h>


namespace Resource
{
	namespace Context
	{
		class Texture : public BaseContextResource
		{
		public:
			Texture();
			~Texture();

			bool load(Manager::ResourceManager* resourceContainer, std::string path);
			char* type() {
				return "texture";
			}
		private:
			std::shared_ptr<Resource::ResourceHandle<Resource::Image>> image;
		};
	}
}

#endif