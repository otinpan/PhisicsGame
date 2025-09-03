#include <glad/glad.h>
#include "rectangle.h"
#include "GLMesh.h"

Rectangle::Rectangle(glm::vec3 center,glm::vec3 rgb,GLMesh& mesh, glm::vec3 scale, float angle)
	:Object(center, rgb,mesh,scale,angle)
{
	setShapeType(SHAPE_RECTANGLE);
}

Rectangle::~Rectangle() {

}

void Rectangle::initialize(class Play* play) {
	Object::initialize(play);
}

void Rectangle::update(float deltaTime) {
	Object::update(deltaTime);
	setAngle(getAngle() + glm::radians(30.0f) * deltaTime);
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