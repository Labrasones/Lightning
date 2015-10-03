#pragma once
#ifndef IMAGE_H
#define IMAGE_H

#include "BaseResource.hpp"

namespace Resource
{
	class Image : public BaseResource
	{
	public:
		Image();
		~Image();

		bool load(Manager::ResourceManager* container, std::string path);

		char* type() {
			char type[10]; // Allocate a return string
			strcat(type, "image_"); // Append type specifier
			return strcat(type, format); // Append and return format specifier
		}

		char* format; // Image format
		unsigned char channels; // Number of channels in image
		unsigned int width, height; // Dimensions of the image
	private:
		unsigned char* _imgdata; // Image data
	};
}

#endif