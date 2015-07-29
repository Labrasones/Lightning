#pragma once
/*
	resource/ResourceHandle.h

	A handle for other parts of the API gain access to a resource. Uses an intermediary pointer 
	to allow for redirection of data without having access to every ResourceHandle to that
	resource
*/
#include <memory>

#include "IntermediaryResourceHandle.h"

namespace Resource
{
	template <class ResourceType>
	class ResourceHandle
	{
	public:
		// Create NULL Handle. Does not point to an intermediary
		ResourceHandle()
		{}
		// Create a Handle to the supplied intermediary handle.
		ResourceHandle(std::shared_ptr<IntermediaryResourceHandle<ResourceType>> intermediaryHandle) : _intermediary(intermediaryHandle)
		{}
		~ResourceHandle()
		{}

		// Set the Handle to the supplied intermediary handle.
		void setIntermediary(std::shared_ptr<IntermediaryResourceHandle<ResourceType>> newIntermediary)
		{
			_intermediary = newIntermediary;
		}

		ResourceType* get()
		{
			if (_intermediary){
				return _intermediary->get();
			}
			else{
				return nullptr;
			}
		}

	private:
		std::shared_ptr<IntermediaryResourceHandle<ResourceType>> _intermediary;
	};
}