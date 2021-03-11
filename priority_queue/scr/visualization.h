#pragma once
#include"shader.h"
#include"renderer.h"
#include"texture.h"
#include"glm/vec2.hpp"
#include<vector>
#include"object.h"
#include<map>
#include"button.h"
#include"scrollbar.h"
#include"tree.h"
#include"text.h"
class Visualization
{
public:
	void init(const int width, const int height);
	void update(float dt);
	void renderer();
	~Visualization();
private:
	void processInput(float dt);
	void savePositions();
	void loadPositions(int i);
	void clearOldPositions();
	void static mouseCallback(GLFWwindow* window, int button, int action, int mods);
	void static keyCallBack(GLFWwindow* window, int key,int scancode, int action,int mods);
	void pushButton();
	void popButton();
	void clearButton();
	void automaticModeButton();
	void stepByStepModeButton();
	Renderer* render;
	Text* text;
	std::vector<Object> circles;
	std::vector<std::vector<Object>> oldPositions;
	std::vector<std::string> comments;
	std::string comment;
	int numberOfStepBack = 0;
	//static Scrollbar scrollbar;
	Tree* tree;
	glm::vec2 startPosition;
	glm::vec4 colorBackground = glm::vec4(0.76f, 0.67f, 0.58f, 0.0f);;
	glm::vec4 defaultColor = glm::vec4(0.04f,0.18f,0.18f,1.0f);
	float intervalY;
	float intervalX_atLastLevel;
	float circleWidth, circleHeight;
	int numOfLevels = 0;
	int width, height;
	bool push = false, pop = false;
	float stepTimer = 0;
	int iForSwap = -1;
	int firstSwap = 0;
	int iCurrent = -1;
	bool swap = false;
	bool wasSwap = false;
	bool comparison = false;
	float speedAnimation = 2.9f;
	bool deleteFirstItem = false;
	int step = 0;
	bool finishedStepMode = true;
};
static bool wasPressMouseButton = false;
static bool wasReleaseMoseButton = false;
static std::vector<Button> buttons;
static std::vector<ScrollBar> scrollBars;
static bool automaticMode = true;
static bool stepByStepMode = false;
static bool nextStep = false;
static bool backStep = false;