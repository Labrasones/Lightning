/*
	core/task/ITaskList.cpp

	Interface for classes which implement a task list
*/
#include "TaskList.hpp"

using namespace Task;

TaskList::TaskList()
{}

TaskList::~TaskList()
{}

void TaskList::addTask(BasicTask* task)
{
	_tasks.push(task);
}

BasicTask* TaskList::popTask()
{
	BasicTask* task = _tasks.top();
	_tasks.pop();
	return task;
}

void TaskList::clearTasks()
{
	while (_tasks.size() > 0)
	{
		_tasks.pop(); // Delete the top task
	}
}