#include "ui.h"
#include "play.h"
#include "uiObject.h"
#include "shader.h"
#include "GLMesh.h"
#include "cup.h"
#include "uiCircle.h"
#include "uiRectangle.h"
#include "uiTriangle.h"

UI::UI()
	:mPlay(nullptr)
{

}

UI::~UI() {

}

void UI::initialize(class Play* play) {
	mPlay = play;

	mUIObjectTopPos = glm::vec3(mPlay->getCup()->getRight() + 0.4f, 0.8f,1.0f);
	mUIObjectMiddlePos = glm::vec3(mUIObjectTopPos - glm::vec3(0, 0.4f, 0.0f));
	mUIObjectBottomPos = glm::vec3(mUIObjectTopPos - glm::vec3(0, 0.7f, 0.0f));


	createUIObject(mUIObjectTopPos);
	createUIObject(mUIObjectMiddlePos);
	createUIObject(mUIObjectBottomPos);

}

void UI::update(float deltaTime) {
	for (auto& obj : mUIObjects) {
		obj->update(deltaTime);
	}

	mUIObjects[0]->setIsTop(true);
	mUIObjects[0]->setPosition(mUIObjectTopPos);
	mUIObjects[1]->setPosition(mUIObjectMiddlePos);
	mUIObjects[2]->setPosition(mUIObjectBottomPos);
}

void UI::drawUIObject(Shader& shader) {
	for (auto& obj : mUIObjects) {
		obj->draw(shader);
	}
}

void UI::createUIObject(glm::vec3 pos) {
	int type = distShapeType(rng);
	float s = distScale(rng2);

	
	switch (type) {
	case(0): {
		UIObject* tri = new UITriangle(
			pos,
			glm::vec3(1.0f, 0.0f, 0.0f),
			mPlay->getTriangleMesh(),
			100.0f,
			0.1f,
			glm::vec3(s, s, 1.0f)
		);
		tri->initialize(this);
		break;
	}
	case(1): {
		UIObject* rec = new UIRectangle(
			pos,
			glm::vec3(0.0f, 1.0f, 0.0f),
			mPlay->getRectangleMesh(),
			100.0f,
			0.1f,
			glm::vec3(s, s, 1.0f)
		);
		rec->initialize(this);
		break;
	}
	case(2): {
		UIObject* cir = new UICircle(
			pos,
			glm::vec3(0.0f, 0.0f, 1.0f),
			mPlay->getCircleMesh(),
			100.0f,
			0.1f,
			glm::vec3(s, s, 1.0f)
		);
		cir->initialize(this);
		break;
	}
	}
}

void UI::addUIObject(UIObject* obj) {
	if (mUIObjects.size() >= 3) {
		printf("UIObject is full\n");
		return;
	}

	mUIObjects.emplace_back(obj);
}


void UI::removeUIObject(UIObject* obj) {
	auto iter = std::find(mUIObjects.begin(), mUIObjects.end(), obj);
	if (iter != mUIObjects.end()) {
		std::iter_swap(iter, mUIObjects.end() - 1);
		mUIObjects.pop_back();
	}
}

objectData UI::takeUIObject() {
	objectData data;
	data.color = mUIObjects[0]->getColor();
	data.scale = mUIObjects[0]->getScale();
	unsigned int type = (unsigned int)mUIObjects[0]->getShapeType();
	data.shapetype = type;

	delete mUIObjects[0];
	std::swap(mUIObjects[1], mUIObjects[0]);

	createUIObject(mUIObjectBottomPos);

	return data;
}