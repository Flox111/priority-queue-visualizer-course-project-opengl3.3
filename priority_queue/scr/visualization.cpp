#include "visualization.h"
#include"object.h"
#include"assets.h"
#include<glm/glm.hpp>
#include<iostream>
#include<ctime>

void Visualization::init(const int _width, const int _height) {
	srand(time(0));
	width = _width;
	height = _height;
	Shader shader("Resources/Shaders/quad_vertex.glsl", "Resources/Shaders/quad_fragment.glsl");
	Assets::shaders["quad_shader"] = shader;
	
	render = new Renderer();
	render->initialize(width, height);

	Texture2D circle;
	circle.loadFromFile("Resources/Textures/circle4.png", true);
	Assets::textures["circle"] = circle;

	Texture2D arrow;
	arrow.loadFromFile("Resources/Textures/arrow.png", true);
	Assets::textures["arrow"] = arrow;

	Texture2D button;
	button.loadFromFile("Resources/Textures/tile_0.png",true);
	Assets::textures["Button"] = button;
	buttons.push_back(Button{ Assets::textures["Button"], glm::vec2(width - 200.0f,height - 50.0f),glm::vec2(30,20),defaultColor,"Push" ,width,height });
	buttons.push_back(Button{ Assets::textures["Button"], glm::vec2(width - 130.0f,height - 50.0f),glm::vec2(30,20),defaultColor,"Pop",width,height });
	buttons.push_back(Button{ Assets::textures["Button"], glm::vec2(width - 270.0f,height - 50.0f),glm::vec2(45,20),defaultColor,"Label",width,height });
	buttons.push_back(Button{ Assets::textures["Button"], glm::vec2(width - 200.0f,height - 90.0f),glm::vec2(100,20),defaultColor,"automaticMode",width,height });
	buttons.back().active = true;
	buttons.push_back(Button{ Assets::textures["Button"], glm::vec2(width - 200.0f,height - 130.0f),glm::vec2(100,20),defaultColor,"stepByStepMode",width,height });
	buttons.push_back(Button{ Assets::textures["Button"], glm::vec2(width - 30.0f,height - 20.0f),glm::vec2(30,20),defaultColor,"Clear",width,height });
	startPosition.x = width / 2;
	startPosition.y = height-50.0f;
	circleWidth = 20.0f; 
	circleHeight = 20.0f;
	intervalY = 2 * circleHeight;
	
	tree = new Tree(startPosition, glm::vec2(circleWidth*2, circleHeight*2));
	glfwSetMouseButtonCallback(Window::window, mouseCallback);
}
void Visualization::update(float dt) {
	stepTimer += speedAnimation * dt;
	timeBetweenTheClicks += 1.5f * dt;
	if (glfwGetKey(Window::window, GLFW_KEY_A) == GLFW_PRESS && timeBetweenTheClicks > 1.0f) {
		if (stepByStepMode) {
			backStep = true;
		}
		timeBetweenTheClicks = 0.0f;
	}
	if (glfwGetKey(Window::window, GLFW_KEY_D) == GLFW_PRESS && timeBetweenTheClicks > 1.0f) {
		if (stepByStepMode) {
			nextStep = true;
		}
		timeBetweenTheClicks = 0.0f;
	}
	if ((stepTimer > 1.0f && automaticMode) || (stepByStepMode && nextStep && finishedStepMode)) {
		stepTimer = 0;
		nextStep = false;
		if (push) {
			if (wasSwap) {
				circles[iCurrent].active2 = false;
				iCurrent = (iCurrent - 1) / 2;
				wasSwap = false;
			}
			else if (swap == true) {
				circles[(iCurrent - 1) / 2].active2 = false;
				circles[(iCurrent - 1) / 2].active = true;
				circles[iCurrent].active = false;
				circles[iCurrent].active2 = true;
				int t = circles[(iCurrent - 1) / 2].value;;
				circles[(iCurrent - 1) / 2].value = circles[iCurrent].value;
				circles[iCurrent].value = t;
				swap = false;
				wasSwap = true;
				comment = "Changing places element " + std::to_string(circles[iCurrent].value) + " and " + std::to_string(circles[(iCurrent - 1) / 2].value);
			}
			else
				if (iCurrent == 0) {
					circles[iCurrent].active = false;
					push = false;
					comment = "The element is at the beginning of the heap, no action is required.";
				}
				else {
					if (circles[iCurrent].value < circles[(iCurrent - 1) / 2].value) {
						circles[iCurrent].active = false;
						push = false;
						comment = "The element "+std::to_string(circles[iCurrent].value) + " less than " + std::to_string(circles[(iCurrent - 1) / 2].value) + ". The climb is over.";
					}
					else {
						circles[(iCurrent - 1) / 2].active2 = true;
						swap = true;
						comment = "The element " + std::to_string(circles[iCurrent].value) + " more than " + std::to_string(circles[(iCurrent - 1) / 2].value) + ".";

					}
				}
			savePositions();
			comments.push_back(comment);
			numberOfStepBack++;
		}
		else if (pop) {
			if (wasSwap) {
				circles[iCurrent].active = false;
				circles[iCurrent].active2 = false;
				circles[iForSwap].active2 = false;
				circles[iForSwap].active = true;
				if (iCurrent * 2 + 2 < circles.size() && iCurrent * 2 + 2 != iForSwap) circles[iCurrent * 2 + 2].active2 = false;
				else if (iCurrent * 2 + 2 == iForSwap) circles[iCurrent * 2 + 1].active2 = false;
				iCurrent = iForSwap;
				wasSwap = false;
			}
			else if (swap) {
				circles[iCurrent].active = false;
				circles[iCurrent].active2 = true;
				circles[iForSwap].active2 = false;
				circles[iForSwap].active = true;
				int t = circles[iCurrent].value;
				circles[iCurrent].value = circles[iForSwap].value;
				circles[iForSwap].value = t;
				swap = false;
				wasSwap = true;
				comment = "Swap the elements " + std::to_string(circles[iCurrent].value) + " and " + std::to_string(circles[iForSwap].value);
			}
			else if (iCurrent * 2 + 1 >= circles.size()) {
				circles[iCurrent].active = false;
				pop = false;
				comment = "The element " + std::to_string(circles[iCurrent].value) + " is at the very bottom, the descent is finished";
			}
			else if (comparison) {
				comparison = false;
				bool temp = iCurrent * 2 + 2 < circles.size() ? 1 : 0;
				if (temp) {
					if (circles[iCurrent].value >= circles[iCurrent * 2 + 1].value &&
						circles[iCurrent].value >= circles[iCurrent * 2 + 2].value) {
						circles[iCurrent].active = false;
						circles[iCurrent * 2 + 1].active2 = false;
						circles[iCurrent * 2 + 2].active2 = false;
						comment = "Element " + std::to_string(circles[iCurrent].value) + " is greater than " + std::to_string(circles[iCurrent * 2 + 1].value) + " and " + std::to_string(circles[iCurrent * 2 + 2].value) + " the descent is complete";
						pop = false;
						return;
					}
					if (circles[iCurrent * 2 + 1].value >= circles[iCurrent * 2 + 2].value) {
						iForSwap = iCurrent * 2 + 1;
					}
					else iForSwap = iCurrent * 2 + 2;
					swap = true;
				}
				else if (circles[iCurrent].value >= circles[iCurrent * 2 + 1].value) {
					circles[iCurrent].active = false;
					circles[iCurrent * 2 + 1].active2 = false;
					pop = false;
				}
				else {
					iForSwap = iCurrent * 2 + 1;
					swap = true;
				}
			}
			else {
				comparison = true;
				circles[iCurrent * 2 + 1].active2 = true;
				if (iCurrent * 2 + 2 < circles.size()) circles[iCurrent * 2 + 2].active2 = true;
				savePositions();
				numberOfStepBack++;
				bool temp = iCurrent * 2 + 2 < circles.size() ? 1 : 0;
				if (temp) comment = "Compare element " + std::to_string(circles[iCurrent].value) + " with " + std::to_string(circles[iCurrent * 2 + 1].value) + " and " + std::to_string(circles[iCurrent * 2 + 2].value);
				else comment = "Compare element " + std::to_string(circles[iCurrent].value) + " with " + std::to_string(circles[iCurrent * 2 + 1].value);
				comments.push_back(comment);
				if (temp) {
					if (circles[iCurrent].value >= circles[iCurrent * 2 + 1].value &&
						circles[iCurrent].value >= circles[iCurrent * 2 + 2].value) {
						return;
					}
					if (circles[iCurrent * 2 + 1].value >= circles[iCurrent * 2 + 2].value) {
						iForSwap = iCurrent * 2 + 1;
					}
					else iForSwap = iCurrent * 2 + 2;
					comparison = false;
					swap = true;
				}
				else if (circles[iCurrent].value >= circles[iCurrent * 2 + 1].value) {
					return;
				}
				else {
					iForSwap = iCurrent * 2 + 1;
					comparison = false;
					swap = true;
				}
				return;
			}
			savePositions();
			numberOfStepBack++;
			comments.push_back(comment);
		}
		else if (deleteFirstItem) {
			switch (step) {
				case 1:
					iCurrent = circles.size() - 1;
					circles[iCurrent].active = true;
					step = 2;
				break;
				case 2:
					if (circles.size() == 1) {
						comment = "The element " + std::to_string(circles.back().value) + " is the only one in the heap, we delete it";
						circles.pop_back();
						step = 0;
						deleteFirstItem = false;
					}
					else {
						iForSwap = 0;
						circles[iForSwap].active2 = true;
						comment = "Take the top element " + std::to_string(circles[iForSwap].value) + " of the heap";
					}
					step = 3;
					break;
				case 3: {
					circles[iCurrent].active = false;
					circles[iCurrent].active2 = true;
					circles[iForSwap].active2 = false;
					circles[iForSwap].active = true;
					int t = circles[iCurrent].value;
					circles[iCurrent].value = circles[iForSwap].value;
					circles[iForSwap].value = t;
					step = 4;
					comment = "Swap the first element " + std::to_string(circles[iForSwap].value) + " and the last " + std::to_string(circles[iCurrent].value) + " in places";
					break;
				}
				case 4: {
					circles[iCurrent].active = false;
					circles[iCurrent].active2 = false;
					circles[iForSwap].active2 = false;
					circles[iForSwap].active = true;
					iCurrent = iForSwap;
					comment = "Deleting the last element " + std::to_string(circles.back().value) + " of the heap";
					circles.pop_back();
					int t = 0;
					for (int i = 0; i < numOfLevels; i++) t += pow(2, i);
					if (circles.size() == t && numOfLevels != 0) {
						tree->reset(--numOfLevels);
						for (int i = 0; i < circles.size(); i++) {
							circles[i].position = tree->positionsOfNodes[i];
						}
					}
					if (circles.size() == 0 || circles.size() == 1) {
						circles[iCurrent].active = false;
					}
					else pop = true;
					step = 0;
					deleteFirstItem = false;
					break;
				}
			}
			savePositions();
			comments.push_back(comment);
			numberOfStepBack++;
		}
	} 
	else if (stepByStepMode){
		if (nextStep) {
			nextStep = false;
			if (numberOfStepBack == oldPositions.size()-1) { finishedStepMode = true; return; }
			loadPositions(++numberOfStepBack);
			comment = comments[numberOfStepBack];
		}
		else if (backStep) {
			finishedStepMode = false;
			backStep = false;
			if (numberOfStepBack == 0) return;
			loadPositions(--numberOfStepBack);
			comment = comments[numberOfStepBack];
		}
	}
	if (!push && !pop) processInput(dt);
}

void Visualization::renderer() {
	render->clear(colorBackground);
	for (int i = 0; i < circles.size();i++) {
		circles[i].drawSelf(*render, circles[(i - 1) / 2].position,i);
	}
	for (auto &t : buttons) {
		t.drawSelf(*render);
	}
	render->drawText(comment, glm::vec2(10.0f, height - 20.0f), 0.4f, defaultColor, false);
}
void Visualization::mouseCallback(GLFWwindow* window, int button, int action, int mods) {

	if (button ==  GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		wasPressMouseButton = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		wasReleaseMoseButton = true;
	}
}
void Visualization::processInput(float dt) {
	timeBetweenTheClicks += 1.5f * dt;
	if (wasPressMouseButton) {
		wasPressMouseButton = false;
		double x, y;
		glfwGetCursorPos(Window::window, &x, &y);
		for (auto& t : buttons) {
			if (t.checkCollision(x, y)) {
				if (t.type != "stepByStepMode" && t.type != "automaticMode") t.active = 1;
				t.wasPress = 1;
			}
		}
		return;
	}
	if (buttons[2].active && timeBetweenTheClicks > 1.0f) {
		if ((glfwGetKey(Window::window, GLFW_KEY_BACKSPACE) == GLFW_PRESS)) {
			if (buttons[2].value.length() > 0) buttons[2].value.pop_back();
			timeBetweenTheClicks = 0;
		}
		if ((glfwGetKey(Window::window, GLFW_KEY_1) == GLFW_PRESS)) {
			buttons[2].value += "1";
			timeBetweenTheClicks = 0;
		}
		if ((glfwGetKey(Window::window, GLFW_KEY_2) == GLFW_PRESS)) {
			buttons[2].value += "2";
			timeBetweenTheClicks = 0;
		}
		if ((glfwGetKey(Window::window, GLFW_KEY_3) == GLFW_PRESS)) {
			buttons[2].value += "3";
			timeBetweenTheClicks = 0;
		}
		if ((glfwGetKey(Window::window, GLFW_KEY_4) == GLFW_PRESS)) {
			buttons[2].value += "4";
			timeBetweenTheClicks = 0;
		}
		if ((glfwGetKey(Window::window, GLFW_KEY_5) == GLFW_PRESS)) {
			buttons[2].value += "5";
			timeBetweenTheClicks = 0;
		}
		if ((glfwGetKey(Window::window, GLFW_KEY_6) == GLFW_PRESS)) {
			buttons[2].value += "6";
			timeBetweenTheClicks = 0;
		}
		if ((glfwGetKey(Window::window, GLFW_KEY_7) == GLFW_PRESS)) {
			buttons[2].value += "7";
			timeBetweenTheClicks = 0;
		}
		if ((glfwGetKey(Window::window, GLFW_KEY_8) == GLFW_PRESS)) {
			buttons[2].value += "8";
			timeBetweenTheClicks = 0;
		}
		if ((glfwGetKey(Window::window, GLFW_KEY_9) == GLFW_PRESS)) {
			buttons[2].value += "9";
			timeBetweenTheClicks = 0;
		}
		if ((glfwGetKey(Window::window, GLFW_KEY_0) == GLFW_PRESS)) {
			buttons[2].value += "0";
			timeBetweenTheClicks = 0;
		}
	}
	if (wasReleaseMoseButton) {
		wasReleaseMoseButton = false;
		for (auto& t : buttons) {
			if (t.wasPress) {
				t.wasPress = false;
				if (t.type == "Push") {
					t.active = 0;
					if (!finishedStepMode) return;
					clearOldPositions();
					savePositions();
					comments.clear();
					comment = "";
					comments.push_back(comment);
					numberOfStepBack = 0;
					push = true;
					iCurrent = circles.size();
					int value = buttons[2].value != "" ? std::stoi(buttons[2].value) : rand() % 100;
					buttons[2].active = false;
					buttons[2].value = "";
					if (circles.size() == 0) {
						circles.push_back({ Assets::textures["circle"], startPosition,glm::vec2(circleWidth,circleHeight),defaultColor,value,true });
						comment = "Add element " + std::to_string(circles.back().value) + " to the end of the heap.";
						comments.push_back(comment);
						savePositions();
						numberOfStepBack++;
						return;
					}
					if (circles.size() == tree->positionsOfNodes.size()) tree->reset(++numOfLevels);
					for (int i = 0; i < circles.size(); i++) {
						circles[i].position = tree->positionsOfNodes[i];
					}
					circles.push_back({ Assets::textures["circle"], tree->positionsOfNodes[circles.size()],glm::vec2(circleWidth,circleHeight),defaultColor,value ,true });
					savePositions();
					numberOfStepBack++;
					comment = "Add element " + std::to_string(circles.back().value) + " to the end of the heap.";
					comments.push_back(comment);
				}
				if (t.type == "Pop" && circles.size() != 0) {
					t.active = 0;
					if (!finishedStepMode || circles.size() == 0) return;
					clearOldPositions();
					comments.clear();
					savePositions();
					comments.push_back("");
					numberOfStepBack = 0;
					deleteFirstItem = true;
					iCurrent = circles.size() - 1;
					circles[iCurrent].active = true;
					savePositions();
					comment = "To remove the top element of the heap, take the last element ";
					comments.push_back(comment);
					numberOfStepBack++;
					step = 2;
				}
				if (t.type == "Clear") {
					t.active = false;
					circles.clear();
					comment = "";
					comments.clear();
					clearOldPositions();
					numberOfStepBack = 0;
					numOfLevels = 0;
					tree->reset(numOfLevels);
				}
				if (t.type == "automaticMode") {
					t.active = true;
					automaticMode = true;
					stepByStepMode = false;
					buttons[4].active = false;
				}
				if (t.type == "stepByStepMode") {
					t.active = true;
					automaticMode = false;
					stepByStepMode = true;
					buttons[3].active = false;
				}
			}
		}
	}
}
void Visualization::savePositions() {
	std::vector<Object> temp;
	for (int i = 0; i < circles.size(); i++) {
		temp.push_back(circles[i]);
	}
	oldPositions.push_back(temp);
}
void Visualization::loadPositions(int i) {
	circles.clear();
	circles = oldPositions[i];
}
void Visualization::clearOldPositions() {
	oldPositions.clear();
}
Visualization::~Visualization() {
	delete render;
	delete tree;
	delete text;
	Assets::dispose();
}
