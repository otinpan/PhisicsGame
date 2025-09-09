#include <glad/glad.h>
#include "uiObject.h"
#include "ui.h"
#define _USE_MATH_DEFINES
#include <math.h>

UIObject::UIObject(glm::vec3 pos, glm::vec3 rgb, GLMesh& mesh, float mass,
	float restitution, glm::vec3 scale)
	:mPosition(pos)
	, mColor(rgb)
	, mMesh(mesh)
	, mMass(mass)
	, mRestitution(restitution)
	, mScale(scale)
	, mUI(0)
{
	mModel = glm::mat4(1.0f);
}

UIObject::~UIObject() {
	mUI->removeUIObject(this);
}

void UIObject::initialize(UI* ui) {
	mUI = ui;

	sVertices = mMesh.vertices;
	mModel = glm::translate(mModel, mPosition);
	mModel = glm::scale(mModel, mScale);

	setModel(mModel);

	if (mUI) mUI->addUIObject(this);
}

void UIObject::update(float deltaTime) {
	float percent = 0.8f;

	if (mIsTop) {
		mScaleTime += deltaTime;
		if (mScaleTime > scaleCycle) {
			mScaleTime = 0.0f;
		}

		percent += sinf(mScaleTime / scaleCycle * M_PI) * 0.1f;
	}

	mModel = glm::mat4(1.0f);
	mModel = glm::translate(mModel, mPosition);
	mModel = glm::scale(mModel, mScale * percent);

	setModel(mModel);

}

void UIObject::draw(Shader& shader) {
}