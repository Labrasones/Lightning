/*
		Lightning Rendering Engine

	main.cpp

	Application entry and window and context creation.
*/
#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>

#include <memory>

#include <Windows.h>

#define GLFW_DLL
#include <GLFW\glfw3.h>

#include "ResourceManager.hpp"
#include "BaseResource.hpp"
#include "ResourceHandle.hpp"
#include "FileManager.hpp"
#include "BasicFile.hpp"
#include "iniTable.hpp"

// TODO: Create generalize error logger
void error_callback(int error, const char* description)
{
	
	std::cout << "Error: " << error << ".\n	" << description << std::endl;
	MessageBox(NULL, description, "Error!", MB_OK);
}

// TODO: Implement a propper imput handler
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(int argc, char *argv)
{
#ifdef WIN32
#ifdef _DEBUG
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
#endif
#endif

	// Initialize GLFW
	if (!glfwInit())
	{
		// TODO: Create generalize error logger
		error_callback(0, "glfw failed to initialize");
		exit(EXIT_FAILURE);
	}
	glfwSetErrorCallback(error_callback);

	// Create the context Object
	Context::WindowContext context(glfwGetPrimaryMonitor());
	
	// Create the file manager
	File::FileManager fileManager;
	fileManager.addSource(new File::Source::BasicFile()); // Add basic File IO as a source

	// Create the resource manager
	Manager::ResourceManager resources(&context, &fileManager);
	
	// Create a fallback iniTable with some data
	Resource::iniTable fallbackTable;
	fallbackTable.SetInt("test", 42); // The key test will give an in, '42'

	// Create a tree for iniTables in the resource manager using the fallback iniTable as a fallback
	resources.createTreeOfType<Resource::iniTable>(fallbackTable, "fallback");


	// Test

	// Get a handle to an iniTable
	Resource::ResourceHandle<Resource::iniTable> testTable; // Handle is empty
	testTable = resources.getResourceHandle<Resource::iniTable>("fallback", false); // Get a handle to the "fallback" resource but do not load it

	// Test the value at key, "test"
	std::cout << "Fallback Table: " << testTable.get()->GetInt("test") << std::endl;



	// Create the window, etc
	context.createWindow(false, true);

	if (!context.getWindow()){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	context.makeCurrent();
	glfwSwapInterval(1);
	glfwSetKeyCallback(context.getWindow(), key_callback);

	while (!glfwWindowShouldClose(context.getWindow()))
	{
		glClearColor(0.2f, 0.1f, 0.1f, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(context.getWindow());
		glfwPollEvents();
	}

	context.destroyWindow();
	glfwTerminate();
	return 0;
}