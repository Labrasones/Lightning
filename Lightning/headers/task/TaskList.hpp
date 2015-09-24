/*
	core/task/ITaskList.h

	Interface for classes which implement a task list
*/
#pragma once

#include <queue>
#include <vector>

#include "BasicTask.hpp"

namespace Task
{
	class TaskList
	{
	public:
		TaskList();
		~TaskList();

		void addTask(BasicTask* task);
		BasicTask* topTask(){
			return _tasks.top();
		}
		BasicTask* popTask();
		void clearTasks();

	private:
		std::priority_queue<BasicTask*, std::vector<BasicTask*>, BasicTask::ComparePriority> _tasks;
	};
}