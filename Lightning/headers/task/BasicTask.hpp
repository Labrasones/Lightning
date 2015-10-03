/*
	core/task/Task.h

	A basic prioritized task.
*/
#pragma once

#include <functional>

namespace Task
{
	// Priority Levels
	enum priority_t {
		LOW			= 0,	// Must be completed before application terminates
		NORMAL		= 1,	// Must be completed during application
		HIGH		= 2,	// Required for application to function nicely
		CRITICAL	= 3,	// Required for applicaiton to function desireably
		NECESSARY	= 4		// Required for application to function correctly
	};

	class BasicTask
	{
	public:
		BasicTask(priority_t priority) : _priority(priority)
		{
			_active = true;
		}
		virtual ~BasicTask();
		virtual void execute() = 0;

		struct ComparePriority : public std::binary_function<BasicTask*, BasicTask*, bool>
		{
			bool operator ()(const BasicTask* left, const BasicTask* right) const
			{
				return left->_priority < right->_priority;
			}
		};

		void cancel(){
			_active = false;
		}
	protected:
		priority_t _priority;
		bool _active;
	};

	inline BasicTask::~BasicTask()
	{}
}