#pragma once
#include "object.h"

class Rectangle :public Object {
public:
	Rectangle(glm::vec2 center, float width, float height, glm::vec3 rgb);
	~Rectangle();

	void initialize() override;
	void update(float deltaTime) override;
	void draw() override;

private:
	float mWidth;
	float mHeight;
	std::vector<glm::vec3> sVertices; //Å‰‚Ì’¸“_À•W

	void rotate();
};