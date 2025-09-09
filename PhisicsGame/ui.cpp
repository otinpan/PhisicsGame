#include "ui.h"
#include "play.h"
#include "uiObject.h"
#include "shader.h"
#include "GLMesh.h"
#include "cup.h"
#include "uiCircle.h"
#include "uiRectangle.h"
#include "uiTriangle.h"
#include "texture.h"
#include <string>

UI::UI()
	:mPlay(nullptr)
	,mScore(0)
	, mEndObject(nullptr)
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
	

	// texture
	sTextureMesh = createInitTexture();

	numTextures.resize(10);
	std::vector<std::string> fileNames = {
		"Assets/zero.png",
		"Assets/one.png",
		"Assets/two.png",
		"Assets/three.png",
		"Assets/four.png",
		"Assets/five.png",
		"Assets/six.png",
		"Assets/seven.png",
		"Assets/eight.png",
		"Assets/nine.png",
	};
	for (int i = 0; i < 10; i++) {
		numTextures[i] = new Texture(
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.15f, 0.15f, 1.0f),
			sTextureMesh,
			0.0f
		);
		numTextures[i]->initialize(fileNames[i].c_str());
	}

}

void UI::update(float deltaTime) {
	for (auto& obj : mUIObjects) {
		obj->update(deltaTime);
	}

	mUIObjects[0]->setIsTop(true);
	mUIObjects[0]->setPosition(mUIObjectTopPos);
	mUIObjects[1]->setPosition(mUIObjectMiddlePos);
	mUIObjects[2]->setPosition(mUIObjectBottomPos);

	// score
	mScoreTextures.clear();


	int nowScore = mScore;
	while (nowScore > 0) {
		int num = nowScore % 10;
		mScoreTextures.emplace_back(numTextures[num]);
		nowScore /= 10;
	}
	if (mScoreTextures.size() == 0) {
		mScoreTextures.emplace_back(numTextures[0]);
	}
}

void UI::drawUIObject(Shader& shader) {
	for (auto& obj : mUIObjects) {
		obj->draw(shader);
	}

	if (mEndObject) {
		mEndObject->draw(shader);
	}
}

void UI::drawTexture(Shader& shader) {
	shader.use();
	if (mEndTextures.size() != 0) {
		glm::vec3 mPos = glm::vec3(0.3f, 0.0f, -1.0f);
		for (auto& tex : mEndTextures) {
			tex->setPosition(mPos);
			tex->setScale(glm::vec3(0.4f, 0.4f, 1.0f));
			tex->draw(shader);
			mPos.x -= 0.4f;

		}
		return;
	}
	glm::vec3 mPos = glm::vec3(
		mPlay->getCup()->getLeft() - 0.3f,
		mPlay->getCup()->getTop() ,
		0.0f);
	for (auto& tex : mScoreTextures) {
		tex->setPosition(mPos);
		tex->draw(shader);
		mPos.x -= 0.15f;
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


	// ƒXƒRƒA‚Ì‰ÁŽZ
	mScore++;

	return data;
}

void UI::endGame() {
	mEndObject = new UIRectangle(
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(1.30f, 1.0f, 1.0f),
		mPlay->getRectangleMesh(),
		0.0f,
		0.0f,
		glm::vec3(2.5f, 1.5f, 1.0f)
	);

	int nowScore = mScore;
	while (nowScore > 0) {
		int num = nowScore % 10;
		mEndTextures.emplace_back(numTextures[num]);
		nowScore /= 10;
	}
	if (mEndTextures.size() == 0) {
		mEndTextures.emplace_back(numTextures[0]);
	}
	printf("endTexture size: %d\n", (int)mEndTextures.size());
}