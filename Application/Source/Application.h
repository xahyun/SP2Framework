
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include <string>

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static void getCursorPosition(double& xpos, double& ypos);
	static void hideCursorWhenInScreen();
	static void setCursorPos(double xpos, double ypos);
	static bool isMouseButtonPressed(int mouseButton);
	static void worldSpaceToScreenSpace(float& xpos, float& ypos);
	static void screenSpaceToWorldSpace(float& xpos, float& ypos);

	//window sizes
	static unsigned m_width;
	static unsigned m_height;

private:

	//Declare a window object
	StopWatch m_timer;
};

#endif