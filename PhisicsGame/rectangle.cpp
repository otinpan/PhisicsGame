#include "rectangle.h"

Rectangle::Rectangle(glm::vec2 center, float width, float height, glm::vec3 rgb)
	:Object(center, rgb)
	, mWidth(width)
	, mHeight(height)
{
	setShapeType(SHAPE_RECTANGLE);
}

Rectangle::~Rectangle() {

}

void Rectangle::initialize() {
	Object::initialize();
	sVertices.resize(4);
	sVertices[0] = glm::vec3(getCenter().x + mWidth / 2.0f, getCenter().y + mHeight / 2.0f, 0.0f);
	sVertices[1] = glm::vec3(getCenter().x + mWidth / 2.0f, getCenter().y - mHeight / 2.0f, 0.0f);
	sVertices[2] = glm::vec3(getCenter().x - mWidth / 2.0f, getCenter().y + mHeight / 2.0f, 0.0f);
	sVertices[3] = glm::vec3(getCenter().x - mWidth / 2.0f, getCenter().y - mHeight / 2.0f, 0.0f);

	setVertices(sVertices);
}

void Rectangle::update(float deltaTime) {
	setCenter(
		glm::vec2(
			getCenter().x + getVelocity().x * deltaTime,
			getCenter().y + getVelocity().y * deltaTime
		)
	);
}

void Rectangle::draw(Shader& shader) {

}

void Rectangle::rotate() {

}