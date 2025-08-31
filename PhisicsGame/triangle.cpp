#include "triangle.h"

Triangle::Triangle(glm::vec2 center, float width, float height, glm::vec3 rgb)
	:Object(center, rgb)
	, mWidth(width)
	, mHeight(height)
{
	setShapeType(SHAPE_TRIANGLE);
}

Triangle::~Triangle() {

}

void Triangle::initialize() {
	Object::initialize();

	sVertices.resize(3);
	sVertices[0] = glm::vec3(getCenter().x + mWidth / 2.0f, getCenter().y - mHeight / 3.0f, 0.0f);
	sVertices[1] = glm::vec3(getCenter().x - mWidth / 2.0f, getCenter().y - mHeight / 3.0f, 0.0f);
	sVertices[2] = glm::vec3(getCenter().x, getCenter().y + (2.0f * mHeight) / 3.0f, 0.0f);

	setVertices(sVertices);
}

void Triangle::update(float deltaTime) {
	setCenter(
		glm::vec2(
		getCenter().x + getVelocity().x * deltaTime,
		getCenter().y + getVelocity().y * deltaTime
		)
	);
}

void Triangle::draw(Shader& shader) {

}

void Triangle::rotate() {

}

