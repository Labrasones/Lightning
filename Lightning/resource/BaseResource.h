/*
	BaseResource.h

	A generic base class for resource types
*/
#pragma once
namespace Resource
{
	class BaseResource
	{
	public:
		BaseResource();
		virtual ~BaseResource() = 0;

		virtual bool load(char* data) = 0;
	};

	inline BaseResource::~BaseResource()
	{

	}
}