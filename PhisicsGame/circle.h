#pragma once
#include "object.h"

class Circle :public Object {
public:
	Circle(glm::vec3 center, float radius, glm::vec3 rgb,GLMesh& mesh);
	~Circle();

	void initialize(class Play* play) override;
	void update(float deltaTime) override;
	void draw(Shader& shader) override;

private:
	float mRadius;
};