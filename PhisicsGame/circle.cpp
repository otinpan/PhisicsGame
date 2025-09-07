#include <glad/glad.h>
#include "Circle.h"
#include "play.h"
#include "cup.h"

Circle::Circle(glm::vec3 center, glm::vec3 rgb, GLMesh& mesh,float mass,float restitution, glm::vec3 scale, float angle)
	:Object(center, rgb, mesh, mass,restitution,scale, angle)
{
	setShapeType(SHAPE_CIRCLE);
	setRadius(mesh.radius*scale.x);
}

Circle::~Circle() {

}

void Circle::initialize(class Play* play) {
	Object::initialize(play);
	// äµê´ÉÇÅ[ÉÅÉìÉg
	setInertia(0.5f * getMass() * getRadius() * getRadius()*0.8f);
	// hitbox
	setHitRadius(getRadius());
}

void Circle::update(float deltaTime) {

	Object::update(deltaTime);

	glm::vec2 offset(0.0f, 0.0f);
	// ç∂ÇÃï«Ç∆è’ìÀ
	if (getCenter().x - getRadius() < getPlay()->getCup()->getLeft()) {
		offset.x = (getPlay()->getCup()->getLeft() - (getCenter().x - getRadius())) * 0.95f;
		solveCollisionCup(glm::vec2(1.0f, 0.0f), glm::vec2(getPlay()->getCup()->getLeft(), getCenter().y));
	}
	// âEÇÃï«Ç∆è’ìÀ
	if (getCenter().x + getRadius() > getPlay()->getCup()->getRight()) {
		offset.x = -((getCenter().x + getRadius()) - getPlay()->getCup()->getRight()) * 0.95f;
		solveCollisionCup(glm::vec2(-1.0f, 0.0f), glm::vec2(getPlay()->getCup()->getRight(), getCenter().y));
	}
	// íÍñ Ç∆è’ìÀ
	if (getCenter().y - getRadius() < getPlay()->getCup()->getBottom()) {
		offset.y = (getPlay()->getCup()->getBottom() - (getCenter().y - getRadius())) * 0.95f;
		solveCollisionCup(glm::vec2(0.0f, 1.0f), glm::vec2(getCenter().x, getPlay()->getCup()->getBottom()));
	}

	setCenter(getCenter() + glm::vec3(offset, 0.0f));
}

void Circle::draw(Shader& shader) {
	shader.use();
	shader.setMatrix4("model", getModel());
	shader.setVec3("objectColor", getColor());
	
	glBindVertexArray(getMesh().VAO);
	glDrawElements(GL_TRIANGLES, getMesh().indexCount, GL_UNSIGNED_INT, 0);
}