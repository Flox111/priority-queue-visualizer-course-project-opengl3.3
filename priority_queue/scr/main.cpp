#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"window.h"
#include"visualization.h"
const int WIDTH = 1200, HEIGHT = 600;
int main() {
	Window::initialize(WIDTH, HEIGHT, "SNAKE");
	Visualization *visual = new Visualization();
	visual->init(WIDTH,HEIGHT);
	double dt = 0.0;
	double last_frame_time = 0.0;
	while (!Window::isShouldClose()) {
		double begin_frame_time = glfwGetTime();
		dt = begin_frame_time - last_frame_time;
		last_frame_time = begin_frame_time;
		glfwPollEvents();
		visual->update(float(dt));
		visual->renderer();
		Window::swapBuffers();
	}
	delete visual;
	Window::Terminate();
}