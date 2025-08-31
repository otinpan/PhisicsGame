#include "Circle.h"

Circle::Circle(glm::vec2 center, float radius, glm::vec3 rgb)
	:Object(center, rgb)
	, mRadius(radius)
{
	setShapeType(SHAPE_CIRCLE);
}

Circle::~Circle() {

}

void Circle::initialize() {
	Object::initialize();
}

void Circle::update(float deltaTime) {
	setCenter(
		glm::vec2(
		getCenter().x + getVelocity().x * deltaTime, 
		getCenter().y + getVelocity().y * deltaTime)
	);
}

void Circle::draw(Shader& shader) {
	
}