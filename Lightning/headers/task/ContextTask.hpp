/*
	core/context/ContextTask.h

	A generic task to be run on the context
*/
#pragma once
#include "BasicTask.hpp"

namespace Context
{
	class ContextTask
	{
	public:
		ContextTask(char priority) : _priority(priority)
		{}
		virtual ~ContextTask();
		virtual void execute() = 0;

		struct ComparePriority
		{
			bool operator ()(const ContextTask*& left, const ContextTask*& right) const
			{
				return left->_priority < right->_priority;
			}
		};
	private:
		char _priority;
	};

	inline ContextTask::~ContextTask()
	{}
}