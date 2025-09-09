#include <glad/glad.h>
#include "uiRectangle.h"

UIRectangle::UIRectangle(glm::vec3 pos, glm::vec3 rgb, GLMesh& mesh, float mass,
	float restitution, glm::vec3 scale)
	:UIObject(pos, rgb, mesh, mass, restitution, scale)
{
	setShapeType(SHAPE_RECTANGLE);
}

UIRectangle::~UIRectangle() {
}

void UIRectangle::initialize(class UI* ui) {
	UIObject::initialize(ui);
}

void UIRectangle::update(float deltaTime) {
	UIObject::update(deltaTime);
}

void UIRectangle::draw(Shader& shader) {
	shader.use();
	shader.setMatrix4("model", getModel());
	shader.setVec3("objectColor", getColor());

	glBindVertexArray(getMesh().VAO);
	glDrawElements(GL_TRIANGLES, getMesh().indexCount, GL_UNSIGNED_INT, 0);
}