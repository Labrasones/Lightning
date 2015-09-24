#pragma once
/*
	resource/IntermediaryHandle.h
*/

#include <memory>

namespace Resource
{
	template <class ResourceType>
	class IntermediaryResourceHandle
	{
	public:
		IntermediaryResourceHandle(ResourceType* resource) : _resource(std::shared_ptr<ResourceType>(resource))
		{}

		~IntermediaryResourceHandle()
		{}

		ResourceType* get()
		{
			return *_resource;
		}

		long use_count()
		{
			_resource.use_count();
		}

	private:
		std::shared_ptr<ResourceType> _resource;
	};
}