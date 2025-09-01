#pragma once
#include "object.h"

class Rectangle :public Object {
public:
	Rectangle(glm::vec3 center, glm::vec3 rgb,GLMesh& mesh, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), float angle = 0.0f);
	~Rectangle();

	void initialize(class Play* play) override;
	void update(float deltaTime) override;
	void draw(Shader& shader) override;

private:

	void rotate();
};