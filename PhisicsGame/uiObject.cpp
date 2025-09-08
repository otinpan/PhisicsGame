#include <glad/glad.h>
#include "uiObject.h"
#include "play.h"

UIObject::UIObject(glm::vec3 pos, glm::vec3 rgb, GLMesh& mesh, float mass,
	float restitution, glm::vec3 scale)
	:mPosition(pos)
	, mColor(rgb)
	, mMesh(mesh)
	, mMass(mass)
	, mRestitution(restitution)
	, mScale(scale)
	, mPlay(0)
{
	mModel = glm::mat4(1.0f);
}

UIObject::~UIObject() {

}

void UIObject::initialize(Play* play) {
	mPlay = play;

	sVertices = mMesh.vertices;
	mModel = glm::translate(mModel, mPosition);
	mModel = glm::scale(mModel, mScale * 0.1f);
}

void UIObject::update(float deltaTime) {
	mModel = glm::mat4(1.0f);
	mModel = glm::translate(mModel, mPosition);
	mModel = glm::scale(mModel, mScale * 0.1f);
}

void UIObject::draw(Shader& shader) {
	shader.use();
	shader.setMatrix4("model", mModel);
	shader.setVec3("objectColor", mColor);

	glBindVertexArray(mMesh.VAO);
	glDrawElements(GL_TRIANGLES, mMesh.indexCount, GL_UNSIGNED_INT, 0);
}