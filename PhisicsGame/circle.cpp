#include "Circle.h"

Circle::Circle(glm::vec3 center, float radius, glm::vec3 rgb,GLMesh&  mesh)
	:Object(center, rgb,mesh)
	, mRadius(radius)
{
	setShapeType(SHAPE_CIRCLE);
}

Circle::~Circle() {

}

void Circle::initialize(class Play* play) {
	Object::initialize(play);
}

void Circle::update(float deltaTime) {
	setCenter(
		glm::vec3(
		getCenter().x + getVelocity().x * deltaTime, 
		getCenter().y + getVelocity().y * deltaTime,
		0.0f
		)
	);
}

void Circle::draw(Shader& shader) {
	
}