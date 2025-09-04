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
}

void Circle::update(float deltaTime) {
	Object::update(deltaTime);

	float e = (float)(getRestitution() + getPlay()->getCup()->getRestitution()) / 2.0f;

	//ç∂ÇÃï«Ç∆è’ìÀ
	if (getCenter().x - getRadius() < getPlay()->getCup()->getLeft()) {
		setCenter(glm::vec3(getPlay()->getCup()->getLeft() + getRadius(), getCenter().y, 0.0f));
		setVelocity(glm::vec2(-getVelocity().x * e, getVelocity().y));
	}
	// âEÇÃï«Ç∆è’ìÀ
	if (getCenter().x + getRadius() > getPlay()->getCup()->getRight()) {
		setCenter(glm::vec3(getPlay()->getCup()->getRight() - getRadius(), getCenter().y, 0.0f));
		setVelocity(glm::vec2(-getVelocity().x * e, getVelocity().y));
	}
	// íÍñ Ç∆è’ìÀ
	if (getCenter().y - getRadius() < getPlay()->getCup()->getBottom()) {
		setCenter(glm::vec3(getCenter().x, getPlay()->getCup()->getBottom() + getRadius(), 0.0f));
		setVelocity(glm::vec2(getVelocity().x, -getVelocity().y * e));
	}

}

void Circle::draw(Shader& shader) {
	shader.use();
	shader.setMatrix4("model", getModel());
	shader.setVec3("objectColor", getColor());
	
	glBindVertexArray(getMesh().VAO);
	glDrawElements(GL_TRIANGLES, getMesh().indexCount, GL_UNSIGNED_INT, 0);
}