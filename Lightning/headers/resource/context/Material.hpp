#pragma once
#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "resource/context/BaseContextResource.hpp"

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
		private:
			// Material parameters go here
		};
	}
}

#endif _MATERIAL_H