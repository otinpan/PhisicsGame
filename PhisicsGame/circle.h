#pragma once
#include "object.h"

class Circle :public Object {
public:
	Circle(glm::vec2 center, float radius, glm::vec3 rgb);
	~Circle();

	void initialize() override;
	void update(float deltaTime) override;
	void draw(Shader& shader) override;

private:
	float mRadius;
};