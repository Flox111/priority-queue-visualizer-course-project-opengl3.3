#pragma once
#include<glm/vec2.hpp>
#include<vector>
class Tree
{
private:
	glm::vec2 startPosition;
	glm::vec2 scale;
	float numOfLevels = 0;
public:
	std::vector<glm::vec2> positionsOfNodes;
	Tree(glm::vec2 _startPostition, glm::vec2 _scale);
	void reset(int _numOfLevels);
};


