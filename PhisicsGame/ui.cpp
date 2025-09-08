#include "ui.h"
#include "play.h"
#include "uiObject.h"
#include "shader.h"
#include "GLMesh.h"
#include "cup.h"

UI::UI()
	:mPlay(nullptr)
{

}

UI::~UI() {
	for (auto& obj : mUIObjects) {
		if (obj) {
			delete obj;
			obj = nullptr;
		}
	}
	mUIObjects.clear();
}

void UI::initialize(class Play* play) {
	mPlay = play;

	// ˆê”Ôã‚ÌˆÊ’u
	mUIObjectBottomPos = glm::vec3(mPlay->getCup()->getRight() + 0.2f, 0.0f,0.0f);

	addUIObject(mUIObjectBottomPos+glm::vec3(0,0.4f,0));
	addUIObject(mUIObjectBottomPos + glm::vec3(0, 0.2f, 0.0f));
	addUIObject(mUIObjectBottomPos);
}

void UI::update(float deltaTime) {

}

void UI::draw(Shader& shader) {
	for (auto& obj : mUIObjects) {
		obj->draw(shader);
	}
}

void UI::addUIObject(glm::vec3 pos) {
	int type = distShapeType(rng);
	float s = distScale(rng);

	switch (type) {
	case(0): {
		UIObject* tri = new UIObject(
			pos,
			glm::vec3(1.0f, 0.0f, 0.0f),
			mPlay->getTriangleMesh(),
			100.0f,
			0.1f,
			glm::vec3(s, s, 1.0f)
		);
		tri->setShapeType(UIObject::SHAPE_TRIANGLE);
		mUIObjects.emplace_back(tri);
		break;
	}
	case(1): {
		UIObject* rec = new UIObject(
			pos,
			glm::vec3(0.0f, 1.0f, 0.0f),
			mPlay->getRectangleMesh(),
			100.0f,
			0.1f,
			glm::vec3(s, s, 1.0f)
		);
		rec->setShapeType(UIObject::SHAPE_RECTANGLE);
		mUIObjects.emplace_back(rec);
		break;
	}
	case(2): {
		UIObject* cir = new UIObject(
			pos,
			glm::vec3(0.0f, 0.0f, 1.0f),
			mPlay->getCircleMesh(),
			100.0f,
			0.1f,
			glm::vec3(s, s, 1.0f)
		);
		cir->setShapeType(UIObject::SHAPE_CIRCLE);
		mUIObjects.emplace_back(cir);
		break;
	}
	}


}

void UI::takeUIObject() {


}