/*
	core/context/Context.h

	Object representing the context and it's state
*/
#pragma once

#include <queue>
#include <time.h>

#define GLFW_DLL
#include "GLFW\glfw3.h"

namespace Context
{
	class WindowContext
	{
	public:
		WindowContext()
		{
			_monitor = NULL;
			_parent = NULL;
			resX = 800;
			resY = 600;
			title = "Lightning";
		}
		WindowContext(GLFWmonitor* targetMonitor) : _monitor(targetMonitor)
		{
			_parent = NULL;
			resX = 800;
			resY = 600;
			title = "Lightning";
		}
		WindowContext(GLFWmonitor* targetMonitor, GLFWwindow* parentWindow) : _monitor(targetMonitor), _parent(parentWindow)
		{
			resX = 800;
			resY = 600;
			title = "Lightning";
		}
		
		~WindowContext()
		{
			if (_window != nullptr)
			{
				destroyWindow();
			}
			//removeTasks();
		}

		// Set the target monitor for next window creation
		void setMonitor(GLFWmonitor* targetMonitor);

		// Set the parent window for next window creation
		void setParent(GLFWwindow* parentWindow);

		// Create the window with current hints
		bool createWindow(bool fullscreen, bool border);

		// Destroy the current window
		void destroyWindow();

		// Set window hint for next window creation
		void setHint(int target, int hint);

		// Make the window context the current one on this thread
		void makeCurrent();

		// Get the current window pointer
		GLFWwindow* getWindow();
		void setWindow(GLFWwindow* window);

		// Get the current window's videomode
		GLFWvidmode* getVideoMode();

		// Add a task to the Task list with the specified priority
		//void addTask(ContextTask* task);

		// Do as many tasks as possible within the budget time. 0=no limit, finish list
		//void doTasks(time_t budget = 0);

		// Remove all tasks fromt the list
		//void removeTasks();

	private:
		// Currently active window
		GLFWwindow* _window;
		GLFWwindow* _parent;
		GLFWmonitor* _monitor;
		unsigned int resX, resY;
		unsigned int posX, posY;
		char* title;

		//std::priority_queue<ContextTask*, std::vector<ContextTask*>, ContextTask::ComparePriority> _tasks;
	};
}