#include <glad/glad.h>
#include "triangle.h"
#include "GLMesh.h"

Triangle::Triangle(glm::vec3 center, glm::vec3 rgb,GLMesh& mesh, float mass,float restitution,glm::vec3 scale, float angle)
	:Object(center, rgb,mesh,mass,restitution,scale,angle)
{
	setShapeType(SHAPE_TRIANGLE);
}

Triangle::~Triangle() {

}

void Triangle::initialize(class Play* play) {
	Object::initialize(play);

}

void Triangle::update(float deltaTime) {
	Object::update(deltaTime);
}

void Triangle::draw(Shader& shader) {
	shader.use();
	shader.setMatrix4("model", getModel());
	shader.setVec3("objectColor", getColor());

	glBindVertexArray(getMesh().VAO);
	glDrawArrays(GL_TRIANGLES, 0, getMesh().vertexCount);

}

void Triangle::rotate() {

}

