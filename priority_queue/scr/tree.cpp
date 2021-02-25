#include "tree.h"

Tree::Tree(glm::vec2 _startPostition, glm::vec2 _scale) {
	startPosition = _startPostition;
	scale = _scale;
	positionsOfNodes.push_back(startPosition);
}
void Tree::reset(int _numOfLevels) {
	numOfLevels = _numOfLevels;
	positionsOfNodes.clear();
	positionsOfNodes.push_back(startPosition);
	float scaleX = scale.x;
	scale.x -= numOfLevels*4.5f;
	int desiredSize = 0;
	for (int i = 0; i <= numOfLevels; i++)
		desiredSize += pow(2, i);
	int n = pow(2, numOfLevels - 1);
	float offsetX = pow(2, numOfLevels) * scale.x;
	float startingPoint = startPosition.x - scale.x / 2 - ((n * scale.x) + ((n - 1) * scale.x));
	for (int i = 1; i <= numOfLevels; i++) {
		float t = ((n * scale.x) + ((n - 1) * scale.x)) / 2;
		float startX = startingPoint + t;
		float posY = startPosition.y - scale.y * i * 2;
		positionsOfNodes.push_back(glm::vec2(startX, posY));
		int j = 1;
		for (int j = 1; j < pow(2, i); j++) {
			positionsOfNodes.push_back(glm::vec2(startX + j * offsetX, posY));
		}
		offsetX /= 2;
		n /= 2;
	}
	scale.x = scaleX;
}