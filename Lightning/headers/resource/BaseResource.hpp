/*
	BaseResource.h

	A generic base class for resource types
*/
#pragma once

#include <iostream>

#include "manager/ResourceManager.hpp"

namespace Resource
{
	class BaseResource
	{
	public:
		BaseResource() : _ready(false), _loading(false), _loaded(false){}
		virtual ~BaseResource() = 0;

		virtual bool load(Manager::ResourceManager* container, std::string path) = 0;

		virtual char* type() = 0;
		virtual bool ready() {
			return _ready;
		}
		virtual bool loading() {
			return _loading;
		}
		virtual bool loaded() {
			return _loaded;
		}

	protected:
		bool _ready;
		bool _loading;
		bool _loaded;
		Manager::ResourceManager* _container;
	};

	inline BaseResource::~BaseResource()
	{

	}
}