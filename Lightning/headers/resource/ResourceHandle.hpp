#pragma once
/*
	resource/ResourceHandle.h
	
	Used to hold a reference to a resource. The resource can be changed on the fly by the resource manager
*/
#include <memory>

namespace Resource
{
	template <class ResourceType>
	class ResourceHandle
	{
	public:
		ResourceHandle(ResourceType *resource) : _res(resource)
		{}
		~ResourceHandle()
		{
			delete _res; // Delete the resource stored in this handle
		}

		void changeResource(ResourceType *resource, bool removeOld = true)
		{
			if( removeOld )
				delete _res; // Delete the old resource stored in this handle
			_res = resource;
		}

		ResourceType* get()
		{
			return _res;
		}

	private:
		ResourceType *_res;
	};
}