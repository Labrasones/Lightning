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

// System headers
#include <GLFW/glfw3.h>


#include "Shader.hpp"
#include "Camera.hpp"
#include "Model.hpp"

// Standard headers
#include <cstdio>
#include <cstdlib>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main(int argc, char *argv)
{

	// Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	File::FileManager file;
	file.addSource(new File::Source::BasicFile()); // Add basic File IO as a source

	Manager::ResourceManager resource(&file);

	// Create a fallback iniTable with some data
	Resource::iniTable fallbackTable;
	fallbackTable.SetInt("fallback", 1);
	// Create a tree for iniTables in the resource manager using the fallback iniTable as a fallback
	resource.addTree<Resource::iniTable>(fallbackTable, "fallback");

	std::shared_ptr<Resource::ResourceHandle<Resource::iniTable>> displaySettings = resource.getHandle<Resource::iniTable>(std::string("display.ini"));

	resource.loadTaskList.popTask()->execute();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//displaySettings->get()->GetInt("major_version"));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//displaySettings->get()->GetInt("minor_version"));
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	if (displaySettings->get()->GetInt("resizeable") == 1) {
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	}
	else {
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	}
	glfwWindowHint(GLFW_SAMPLES, displaySettings->get()->GetInt("samples"));

	Resource::iniTable* resTable = displaySettings->get()->GetSubtable("resolution");
	GLFWwindow* window;
	if (resTable) {
		window = glfwCreateWindow(resTable->GetInt("x"), resTable->GetInt("y"), "Lightning OpenGl Renderer", nullptr, nullptr);
	}
	else {
		fprintf(stderr, "Invalid Display INI Setup");
		return EXIT_FAILURE;
	}

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Check for Valid Context
	if (window == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		return EXIT_FAILURE;
	}

	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(window);
	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	//glViewport(0, 0, resTable.GetInt("x"), resTable.GetInt("y"));

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	/*
		INITIALIZATION
	*/
	
	Shader ourShader("vertexShader.vert", "fragshader.frag");
	Model ourModel("resource/nanosuit.obj");

	float screenWidth = (float)resTable->GetInt("x");
	float screenHeight = (float)resTable->GetInt("y");

	// Game loop
	glClearColor(0.25f, 0.3f, 0.25f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();

		// Clear the colorbuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw our first triangle
		ourShader.Use();

		// Transformation matrices
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Draw the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		ourModel.Draw(ourShader);

		// Swap the buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glfwTerminate();
	return EXIT_SUCCESS;
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}