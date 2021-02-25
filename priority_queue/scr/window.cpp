#include "window.h"
#include<iostream>
GLFWwindow* Window::window;
void framebuffer_size_callback(GLFWwindow* window,int width,int height);
int Window::initialize(const int width, const int height,const char *title) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width,height,title,NULL,NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD";
		return -1;
	}
	glViewport(0,0,width,height);
}
void Window::swapBuffers() {
	glfwSwapBuffers(window);
}
bool Window::isShouldClose() {
	return glfwWindowShouldClose(window);
}
void Window::Terminate() {
	glfwTerminate();
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}