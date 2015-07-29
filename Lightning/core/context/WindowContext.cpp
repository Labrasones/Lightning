/*
	core/context/Context.cpp

	Object representing the context and it's state
*/
#include "WindowContext.h"
using namespace Context;

bool WindowContext::createWindow(bool fullscreen, bool border)
{
	if (_window != NULL) // My window already exists, destroy it first
	{
		destroyWindow();
	}
	if (fullscreen && _monitor!=NULL)
	{ // Fullscreen
		if (border) // Regular fullscreen
		{
			_window = glfwCreateWindow(resX, resY, title, _monitor, _parent);
		}
		else // Borderless fullscreen
		{
			const GLFWvidmode* mode = glfwGetVideoMode(_monitor);

			setHint(GLFW_RED_BITS, mode->redBits);
			setHint(GLFW_GREEN_BITS, mode->greenBits);
			setHint(GLFW_BLUE_BITS, mode->blueBits);
			setHint(GLFW_REFRESH_RATE, mode->refreshRate);

			_window = glfwCreateWindow(mode->width, mode->height, title, _monitor, _parent);
		}
	}
	else // Not fullscreen, or no target monitor
	{
		if (!border)
		{ // Borderless window
			setHint(GLFW_DECORATED, GL_FALSE);
		}
		_window = glfwCreateWindow(resX, resY, title, NULL, _parent);
	}
	if (_window && _monitor != NULL)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(_monitor);
		glfwSetWindowPos(_window, mode->width / 2 - resX / 2, mode->height / 2 - resY / 2);
	}

	if (_window)
		return true;
	else
		return false;
}

void WindowContext::destroyWindow()
{
	glfwDestroyWindow(_window);
	_window = nullptr;
}

void WindowContext::setHint(int target, int hint)
{
	glfwWindowHint(target, hint);
}

void WindowContext::makeCurrent()
{
	glfwMakeContextCurrent(_window);
}

GLFWwindow* WindowContext::getWindow()
{
	return _window;
}

void WindowContext::setWindow(GLFWwindow* window)
{
	_window = window;
}
/*void WindowContext::addTask(ContextTask* task)
{
	_tasks.push(task);
}*/

/*void WindowContext::doTasks(time_t budget = 0)
{
	time_t start = clock();
	while (_tasks.size() > 0 && (clock() - start) < budget) // While there are tasks left and we havn't gone past the budgeted time
	{
		ContextTask* curTask = _tasks.top(); // Get the highest priority task
		_tasks.pop();
		curTask->execute();
	}
}*/