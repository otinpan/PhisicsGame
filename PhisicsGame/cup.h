#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "input.h"

class Cup {
public:
	Cup(glm::vec2 center,float width,float height,glm::vec3 color);
	~Cup();

	void initialize();
	void update(float deltaTime);
	void processInput(const InputState& state);
	void draw();

private:
	glm::vec2 mCenter;
	float mWidth;
	float mHeight;
	glm::vec3 mColor;

	std::vector<glm::vec3> mVertices;
};