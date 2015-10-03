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
		BaseResource() : _ready(false), _loading(false), _loaded(false), _failed(false){}
		virtual ~BaseResource() = 0;

		virtual bool load(Manager::ResourceManager* container, std::string path) = 0;
		virtual char* type() = 0;

		bool ready() {
			return _ready;
		}
		// Change the state of the resource to not ready.
		void makeUnready() {
			_ready = false;
		}
		bool loading() {
			return _loading;
		}
		bool loaded() {
			return _loaded;
		}
		bool failed() {
			return _failed;
		}

	protected:
		bool _ready;
		bool _loading;
		bool _loaded;
		bool _failed;
	};

	inline BaseResource::~BaseResource()
	{

	};
}