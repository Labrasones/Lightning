/*
		Lightning Rendering Engine

	main.cpp
*/

// Local headers
#include "lightning.hpp"

#include "manager/ResourceManager.hpp"
#include "resource/BaseResource.hpp"
#include "resource/ResourceHandle.hpp"
#include "manager/FileManager.hpp"
#include "file/BasicFile.hpp"
#include "resource/iniTable.hpp"
#include "glad/glad.h"

// System headers
#include <GLFW/glfw3.h>

// Standard headers
#include <cstdio>
#include <cstdlib>

// TODO: Create generalize error logger
void error_callback(int error, const char* description)
{
	
	std::cout << "Error: " << error << ".\n	" << description << std::endl;
}

// TODO: Implement a propper imput handler
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(int argc, char *argv)
{

	// Initialize GLFW
	if (!glfwInit())
	{
		error_callback(0, "glfw failed to initialize");
		exit(EXIT_FAILURE);
	}
	glfwSetErrorCallback(error_callback);

	File::FileManager file;
	file.addSource(new File::Source::BasicFile()); // Add basic File IO as a source

	Manager::ResourceManager resource(&file);

	// Create a fallback iniTable with some data
	Resource::iniTable fallbackTable;
	fallbackTable.SetInt("fail", 1); // The key test will give an in, '42'
	// Create a tree for iniTables in the resource manager using the fallback iniTable as a fallback
	resource.createTreeOfType<Resource::iniTable>(fallbackTable, "fallback");

	std::shared_ptr<Resource::ResourceHandle<Resource::iniTable>> displaySettings = resource.getHandle<Resource::iniTable>(std::string("display.ini"));

	resource.loadTaskList.popTask()->execute();

	// Get a handle to an iniTable
	std::cout << "Test Value: " << displaySettings->get()->GetInt("test") << std::endl;

	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	auto mWindow = glfwCreateWindow(1000, 800, "OpenGL", nullptr, nullptr);

	// Check for Valid Context
	if (mWindow == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		return EXIT_FAILURE;
	}

	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(mWindow);
	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	// Rendering Loop
	while (glfwWindowShouldClose(mWindow) == false) {
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(mWindow, true);

		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Flip Buffers and Draw
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}   glfwTerminate();
	return EXIT_SUCCESS;

	/*
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
	return 0;*/
}