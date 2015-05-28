/*
		Lightning Rendering Engine

	main.cpp

	Application entry and window and context creation.
*/

#include <iostream>

#define GLFW_DLL
#include "GLFW\glfw3.h"


#include "core\iniTable.h"
#include "core\fio\Hierarchy.h"
#include "core\fio\sources\BasicFile.h"

// TODO: Create generalize error logger
void error_callback(int error, const char* description)
{
	std::cout << "Error: " << error << ".\n	" << description << std::endl;
}

// TODO: Implement a propper imput handler
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(int argc, char* argv)
{
	File::Hierarchy::addSource(new File::Source::BasicFile());
	Util::iniTable displayIni;
	displayIni.LoadIniFile("config/display.ini");

	if (!glfwInit())
	{
		// TODO: Create generalize error logger
		error_callback(0, "glfw failed to initialize");
		exit(EXIT_FAILURE);
	}

	GLFWwindow* gameWindow;

	glfwSetErrorCallback(error_callback);

	// Window Hints
		// Border
	if (displayIni.GetInt("noborder") != 0){
		glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	}
	if (displayIni.GetInt("resizeable") == 0){
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	}

		// OpenGL Version
	// TODO: Multiple OpenGL version support needs to be implemented here
	int verMaj = displayIni.GetInt("major_version");
	if (verMaj == 3){
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, verMaj);
	}
	else{
		// TODO: Create generalize error logger
		error_callback(0, "Lightning does not support this OpenGL major version");
	}
	int verMin = displayIni.GetInt("minor_version");
	if (verMin == 3){
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, verMin);
	}
	else{
		// TODO: Create generalize error logger
		error_callback(0, "Lightning does not support this OpenGL minor version");
	}

		// Sampling
	if (displayIni.ExistsInt("samples")){
		glfwWindowHint(GLFW_SAMPLES, displayIni.GetInt("samples"));
	}

	int resX, resY;
	resX = displayIni.GetSubtable("resolution").GetInt("x");
	resY = displayIni.GetSubtable("resolution").GetInt("y");
	if (displayIni.GetInt("fullscreen") != 0){ // Fullscreen Creation
		gameWindow = glfwCreateWindow(resX, resY, "Lightning Rendering Engine", glfwGetPrimaryMonitor(), NULL);
	}
	else if(displayIni.GetInt("fullscreenborderless") != 0){ // Borderless full screen
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		gameWindow = glfwCreateWindow(mode->width, mode->height, "Lightning Rendering Engine", glfwGetPrimaryMonitor(), NULL);
	}
	else // Regular window
	{
		gameWindow = glfwCreateWindow(resX, resY, "Lightning Rendering Engine", NULL, NULL);
		if (gameWindow){
			// Position the window in the center of the screen.
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowPos(gameWindow, mode->width/2 - resX/2, mode->height/2 - resY/2);
		}
	}
	
	if (!gameWindow){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(gameWindow);
	glfwSwapInterval(1);
	glfwSetKeyCallback(gameWindow, key_callback);

	while (!glfwWindowShouldClose(gameWindow))
	{
		glClearColor(0.2f, 0.1f, 0.1f, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glfwSwapBuffers(gameWindow);
		glfwPollEvents();
	}

	glfwDestroyWindow(gameWindow);
	glfwTerminate();

	return 1;
}