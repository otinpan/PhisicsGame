#include <glad/glad.h>
#include "uiTriangle.h"

UITriangle::UITriangle(glm::vec3 pos, glm::vec3 rgb, GLMesh& mesh, float mass,
	float restitution, glm::vec3 scale)
	:UIObject(pos, rgb, mesh, mass, restitution, scale)
{
	setShapeType(SHAPE_TRIANGLE);
}

UITriangle::~UITriangle() {
}

void UITriangle::initialize(class UI* ui) {
	UIObject::initialize(ui);
}

void UITriangle::update(float deltaTime) {
	UIObject::update(deltaTime);
}

void UITriangle::draw(Shader& shader) {
	shader.use();
	shader.setMatrix4("model", getModel());
	shader.setVec3("objectColor", getColor());

	glBindVertexArray(getMesh().VAO);
	glDrawArrays(GL_TRIANGLES, 0, getMesh().vertexCount);
}
