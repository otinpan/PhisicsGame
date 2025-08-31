#pragma once
#include "object.h"

class Triangle :public Object {
public:
	Triangle(glm::vec2 center, float width, float height, glm::vec3 rgb);
	~Triangle();

	void initialize() override;
	void update(float deltaTime) override;
	void draw(Shader& shader) override;

private:
	float mWidth;
	float mHeight;

	std::vector<glm::vec3> sVertices; //Å‰‚Ì’¸“_À•W

	void rotate();
};