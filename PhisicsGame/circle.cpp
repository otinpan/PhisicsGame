#include <glad/glad.h>
#include "Circle.h"

Circle::Circle(glm::vec3 center, glm::vec3 rgb, GLMesh& mesh, glm::vec3 scale, float angle)
	:Object(center, rgb, mesh, scale, angle)
{
	setShapeType(SHAPE_CIRCLE);
	mRadius = mesh.radius;
}

Circle::~Circle() {

}

void Circle::initialize(class Play* play) {
	Object::initialize(play);
}

void Circle::update(float deltaTime) {
	Object::update(deltaTime);
}

void Circle::draw(Shader& shader) {
	shader.use();
	shader.setMatrix4("model", getModel());
	shader.setVec3("objectColor", getColor());
	
	glBindVertexArray(getMesh().VAO);
	glDrawElements(GL_TRIANGLES, getMesh().indexCount, GL_UNSIGNED_INT, 0);
}