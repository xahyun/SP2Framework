
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "clock.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

unsigned Application::m_height;
unsigned Application::m_width;

gameManager GameManager;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h) {

	//this does not work lol
	Application::m_height = h;
	Application::m_width = w;
	
	glViewport(0, 0, w, h); //update opengl the new window size
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

void Application::getCursorPosition(double& xpos, double& ypos)
{
	glfwGetCursorPos(m_window, &xpos, &ypos);
}

void Application::hideCursorWhenInScreen()
{
	if (!glfwGetWindowAttrib(m_window, GLFW_HOVERED)) {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
}


void Application::setCursorPos(double xpos, double ypos)
{
	glfwSetCursorPos(m_window, xpos, ypos);
}

bool Application::isMouseButtonPressed(int mouseButton)
{
	return glfwGetMouseButton(m_window, mouseButton) != 0;
}

void Application::worldSpaceToScreenSpace(double& xpos, double& ypos)
{
	xpos = (xpos * 80) / Application::m_width;
	ypos = 60 - ((ypos * 60) / Application::m_height);
}

void Application::screenSpaceToWorldSpace(double& xpos, double& ypos)
{
	xpos = (xpos * Application::m_width) / 80;
	ypos = (ypos * Application::m_height) / 60;
}


Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	m_width = 1000;
	m_height = 750;

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_width, m_height, "Pls let me pass COMG", NULL, NULL);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}


}

void Application::Run()
{
	//Main Loop
	GameManager.Init();
	GameManager.StartGame();

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !GameManager.isFinishedGame)
	{
		GameManager.Update(m_timer.getElapsedTime());
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.

	} //Check if the ESC key had been pressed or if the window had been closed

	GameManager.FinishGame();
	GameManager.ExitGame();
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
