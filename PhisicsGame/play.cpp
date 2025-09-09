#include "play.h"
#include "object.h"
#include "cup.h"
#include "triangle.h"
#include "rectangle.h"
#include "circle.h"
#include "intersect.h"
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>
#include "ui.h"
#include "uiObject.h"
#define _USE_MATH_DEFINES
#include <math.h>



Play::Play()
	:mUpdatingObjects(false)
	, mIsRunning(true)
	, mIsHitstop(false)
	, hitstopTime(0.0f)
	, mHitstopTime(0.0f)
	, mSeqID(Parent::SEQ_NONE)
	, mShader(nullptr)
	, mCup(nullptr)
	, mIsTriangleCooldown(false)
	, mIsRectangleCooldown(false)
	, mIsCircleCooldown(false)
	, mIsObjectCooldown(false)
	, mTriangleAccumulator(0.0f)
	, mRectangleAccumulator(0.0f)
	, mCircleAccumulator(0.0f)
{
	initialize();
}

Play::~Play() {
	shutdown();
}

bool Play::initialize() {
	mLastTime = std::chrono::steady_clock::now();
	mAccumulator = 0.0f;
	loadData();
	return true;
}

void Play::update(Parent* parent) {
	if (mIsRunning) {
		if (mSeqID != Parent::SEQ_NONE) {
			moveTo(parent, mSeqID);
		}

		auto now = std::chrono::steady_clock::now(); // 現在の時間
		float frameTime = std::chrono::duration<float>(now - mLastTime).count(); // 前フレームからの経過時間を秒で取得
		mLastTime = now;

		frameTime = std::min(frameTime, 0.25f);
		mAccumulator += frameTime;


		if (!mIsEnd) {
			int steps = 0;
			const int kMaxSteps = 5;
			while (mAccumulator >= deltaTime && steps < kMaxSteps) {
				processInput(parent, deltaTime);
				updatePlay(deltaTime);
				mUI->update(deltaTime);
				mAccumulator -= deltaTime;
				++steps;
			}
		}

		draw();
	}
}

void Play::processInput(Parent* parent,float deltaTime) {
	if (!parent) {
		printf("No parent\n");
		throw;
	}
	
	parent->gatherInput(mInputState);



	// Objectの追加
	if (mIsObjectCooldown) {
		mObjectAccumulator += deltaTime;
		if (mObjectAccumulator >= 1.0f) {
			mIsObjectCooldown = false;
			mObjectAccumulator = 0.0f;
		}
	}
	float posx = std::min(mInputState.mouseX, mCup->getRight() - 0.1f);
	posx = std::max(posx, mCup->getLeft() + 0.1f);
	float posy = mCup->getTop() + 0.3f;

	// mHaveObjectの更新
	mHaveObject->setPosition(glm::vec3(posx, posy, 0.0f));
	mHaveObject->setCenter(glm::vec3(posx, posy, 0.0f));

	
	if (mInputState.key[GLFW_KEY_LEFT]) {
		mHaveObject->setAngle(mHaveObject->getAngle() + M_PI / 5.0f * deltaTime);
	}
	if (mInputState.key[GLFW_KEY_RIGHT]) {
		mHaveObject->setAngle(mHaveObject->getAngle() - M_PI / 5.0f * deltaTime);
	}

	if (mInputState.mouseLeftPressed && !mIsObjectCooldown) {
		// ボタンを押したら発射
		mHaveObject->setGravity(glm::vec2(0.0f, -0.1f));

		// uiの一番上のオブジェクトを追加
		objectData data = mUI->takeUIObject();
		if (data.shapetype == 1)
			addTriangle(
				glm::vec3(posx,posy, 0.0f),
				data.color,
				100.0f,
				0.1f,
				data.scale,
				0.0f
			);
		else if (data.shapetype == 2)
			addRectangle(
				glm::vec3(posx, posy, 0.0f),
				data.color,
				100.0f,
				0.1f,
				data.scale,
				0.0f
			);
		else if (data.shapetype ==0)
			addCircle(
				glm::vec3(posx, posy, 0.0f),
				data.color,
				100.0f,
				0.1f,
				data.scale
			);
		mIsObjectCooldown = true;

	}

}

void Play::updatePlay(float deltaTime) {
	if (mIsHitstop) {
		updateHitstop(deltaTime);
		return;
	}

	mUpdatingObjects = true;
	for (auto &obj : mObjects) {
		obj->update(deltaTime);
	}
	mUpdatingObjects = false;

	for (auto pending : mPendingObjects) {
		mObjects.emplace_back(pending);
	}
	for (int k = 0; k < 5; k++) {
		for (int i = 0; i < mObjects.size(); i++) {
			for (int j = i + 1; j < mObjects.size(); j++) {
				if (mObjects[i] == mHaveObject || mObjects[j] == mHaveObject)continue;
				isIntersects(mObjects[i], mObjects[j]);
			}
		}
	}
	mPendingObjects.clear();

	std::vector<Object*> deadObjects;
	for (auto obj : mObjects) {
		if (obj->getState() == Object::DEAD) {
			deadObjects.emplace_back(obj);
		}
	}

	for (auto object : deadObjects) {
		delete object;
	}

	mCup->update(deltaTime);
	
}

void Play::draw() {
	if (!mShader) {
		std::cerr << "Shader not initialized!" << std::endl;
		return;
	}
	if (!mCup) {
		std::cerr << "Cup not initialized!" << std::endl;
		return;
	}
	unsigned int mScreenWidth, mScreenHeight;
	glfwGetFramebufferSize(glfwGetCurrentContext(), (int*)&mScreenWidth, (int*)&mScreenHeight);
	if (mScreenWidth == 0 || mScreenHeight == 0) {
		mScreenWidth = 800;
		mScreenHeight = 600;
		std::cerr << "Screen size is zero!" << std::endl;
		printf("%d %d\n", mScreenWidth, mScreenHeight);
	}


	mShader->use();
	float aspect = (float)mScreenWidth / (float)mScreenHeight;
	glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
	mShader->setMatrix4("projection", projection);

	mTextureShader->use();
	mTextureShader->setMatrix4("projection", projection);

	for (auto& obj : mObjects) {
		if (obj) obj->draw(*mShader);
	}
	mCup->draw(*mShader);


	
	mUI->drawUIObject(*mShader);
	mUI->drawTexture(*mTextureShader);


}

void Play::endGame() {
	mIsEnd = true;
	mUI->endGame();

}


void Play::setHitstopTime(float time) {
	mHitstopTime = 0.0f;
	hitstopTime = time;
	mIsHitstop = true;
}

void Play::updateHitstop(float deltaTime) {
	if (mIsHitstop) {
		if (mHitstopTime > hitstopTime) {
			mIsHitstop = false;
		}
		else {
			mHitstopTime += deltaTime;
		}
	}
}



void Play::loadData() {

	// Shader
	mShader = new Shader("vertex_normal.glsl", "fragment_normal.glsl");
	mTextureShader = new Shader("vertex_texture.glsl", "fragment_texture.glsl");

	// Cup
	mCup = new Cup();
	mCup->initialize(this);


	//Object
	sTriangleMesh = createInitTriangle();
	sRectangleMesh = createInitRectangle();
	sCircleMesh = createInitCircle(64, 0.5f);

	// UI 
	mUI = new UI();
	mUI->initialize(this);

	// Triangle
	float s = distScale(rng);
	int type = distShapeType(rng);
	float posy = mCup->getTop() + 0.3f;
	if(type==0){
		addTriangle(
			glm::vec3(0.0f,posy , 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			100.0f,
			0.1f,
			glm::vec3(s, s, 1.0f),
			0.0f
		);

	}
	// Rectangle
    else if(type==1){
		addRectangle(
			glm::vec3(0.0f,posy, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			100.0f,
			0.1f,
			glm::vec3(s, s, 1.0f),
			0.0f
		);
	}
	else if(type==2){
		addCircle(
			glm::vec3(0.0f, posy, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			100.0f,
			0.1f,
			glm::vec3(s, s, 1.0f)
		);

	}
}

void Play::unloadData() {
	for (auto& obj : mObjects) {
		obj->setState(Object::DEAD);
	}
	mCup = nullptr;
	delete mCup;

	mUI = nullptr;
	delete mUI;
}

void Play::shutdown() {
	unloadData();
}

void Play::addObject(Object* obj) {
	if (mUpdatingObjects) {
		mPendingObjects.push_back(obj);
	}
	else {
		mObjects.push_back(obj);
	}
}

void Play::removeObject(Object* obj) {
	auto iter = std::find(mObjects.begin(), mObjects.end(), obj);
	if (iter != mPendingObjects.end()) {
		std::iter_swap(iter, mPendingObjects.end() - 1);
		mPendingObjects.pop_back();
	}

	iter = std::find(mObjects.begin(), mObjects.end(), obj);
	if (iter != mObjects.end()) {
		std::iter_swap(iter, mObjects.end() - 1);
		mObjects.pop_back();
	}
}


void Play::addTriangle(glm::vec3 center, glm::vec3 color,float mass,float restitution, glm::vec3 scale, float angle) {
	Triangle* tri = new Triangle(center, color, sTriangleMesh, mass,restitution, scale, angle);
	tri->initialize(this);
}

void Play::addRectangle(glm::vec3 center, glm::vec3 color, float mass, float restitution, glm::vec3 scale, float angle) {
	Rectangle* rect = new Rectangle(center, color, sRectangleMesh, mass,restitution,scale, angle);
	rect->initialize(this);
}

void Play::addCircle(glm::vec3 center, glm::vec3 color, float mass, float restitution, glm::vec3 scale, float angle) {
	Circle* cir = new Circle(center, color, sCircleMesh, mass,restitution,scale, angle);
	cir->initialize(this);
}


void Play::setHaveObject(Object* obj) {
	mHaveObject = obj;
}
void Play::moveTo(Parent* parent, Parent::SeqID id) {
	
}