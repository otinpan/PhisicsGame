#include <glad/glad.h>
#include "rectangle.h"
#include "GLMesh.h"

Rectangle::Rectangle(glm::vec3 center,glm::vec3 rgb,GLMesh& mesh, float mass,float restitution,glm::vec3 scale, float angle)
	:Object(center, rgb,mesh,mass,restitution,scale,angle)
{
	setShapeType(SHAPE_RECTANGLE);
}

Rectangle::~Rectangle() {

}

void Rectangle::initialize(class Play* play) {
	Object::initialize(play);

	// äµê´ÉÇÅ[ÉÅÉìÉg
	setInertia(getMass() / 6.0f * (getScale().x * getScale().x + getScale().y * getScale().y)*0.8f);
	
	// hitbox
	float hitRadius = 0.0f;
	for (auto& v : getVertices()) {
		hitRadius = std::max(hitRadius, glm::length(getPosition() - v));
	}

	setHitRadius(hitRadius);

}

void Rectangle::update(float deltaTime) {
	Object::update(deltaTime);
	collisionCup();
}

void Rectangle::draw(Shader& shader) {
	shader.use();
	shader.setMatrix4("model", getModel());
	shader.setVec3("objectColor", getColor());

	glBindVertexArray(getMesh().VAO);
	glDrawElements(GL_TRIANGLES, getMesh().indexCount, GL_UNSIGNED_INT, 0);
}

void Rectangle::rotate() {

}