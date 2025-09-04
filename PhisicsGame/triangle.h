#pragma once
#include "object.h"

class Triangle :public Object {
public:
	Triangle(glm::vec3 center,glm::vec3 rgb,GLMesh& mesh, float mass,float restitution,
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), float angle = 0.0f);
	~Triangle();

	void initialize(class Play* play) override;
	void update(float deltaTime) override;
	void draw(Shader& shader) override;

private:
	void rotate();

	unsigned int VAO, VBO;
};