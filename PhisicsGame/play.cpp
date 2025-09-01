#include "play.h"
#include "object.h"
#include "cup.h"
#include "triangle.h"
#include "rectangle.h"
#include "circle.h"
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


Play::Play()
	:mUpdatingObjects(false)
	, mIsRunning(true)
	, mIsHitstop(false)
	, hitstopTime(0.0f)
	, mHitstopTime(0.0f)
	, mSeqID(Parent::SEQ_NONE)
	, mShader(nullptr)

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

		processInput(parent);

		int steps = 0;
		const int kMaxSteps = 5;
		while (mAccumulator >= deltaTime && steps < kMaxSteps) {
			updatePlay(deltaTime);
			mAccumulator -= deltaTime;
			++steps;
		}

		draw();
	}
}

void Play::processInput(Parent* parent) {
	if (!parent) {
		printf("No parent\n");
		throw;
	}
	InputState state;
	parent->gatherInput(state);
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
	for (auto& obj : mObjects) {
		obj->draw(*mShader);
	}
	mCup->draw();
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

	mCup = new Cup(glm::vec2(0.0f, -0.5f), 1.0f, 0.5f, glm::vec3(0.5f, 0.35f, 0.05f));
	mCup->initialize();

	sTriangleMesh = createInitTriangle();
	sRectangleMesh = createInitRectangle();

	Triangle* tri = new Triangle(glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(1.0f, 0.0f, 0.0f), sTriangleMesh);
	tri->initialize(this);

	Rectangle* rect = new Rectangle(
		glm::vec3(0.0f, 0.5f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		sRectangleMesh,
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::radians(45.0f)
	);
	rect->initialize(this);
	rect->setVelocity(glm::vec2(1.0f, 0.0f));


	printf("%d", mObjects.size());
}

void Play::unloadData() {
	while (!mObjects.empty()) {
		delete mObjects.back();
	}
	mCup = nullptr;
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


void Play::moveTo(Parent* parent, Parent::SeqID id) {
	
}