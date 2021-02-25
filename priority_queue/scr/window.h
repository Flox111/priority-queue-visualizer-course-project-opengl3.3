#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
class Window
{
public:
	static GLFWwindow* window;
	static int initialize(const int width, const int height, const char* title);
	static bool isShouldClose();
	static void swapBuffers();
	static void Terminate();
};

