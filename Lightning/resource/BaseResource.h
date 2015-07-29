/*
	BaseResource.h

	A generic base class for resource types
*/
#pragma once

#include <iostream>

#include "../manager/ResourceManager.h"

namespace Resource
{
	class BaseResource
	{
	public:
		BaseResource(){}
		virtual ~BaseResource() = 0;

		virtual bool load(Manager::ResourceManager* container, std::string path) = 0;
		

		virtual char* type() = 0;
		virtual bool is_ready() = 0;

	protected:
		bool _ready;
		Manager::ResourceManager* _container;
	};

	inline BaseResource::~BaseResource()
	{

	}
}