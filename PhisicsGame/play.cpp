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


		int steps = 0;
		const int kMaxSteps = 5;
		while (mAccumulator >= deltaTime && steps < kMaxSteps) {
			processInput(parent,deltaTime);
			updatePlay(deltaTime);
			mAccumulator -= deltaTime;
			++steps;
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
	// Triangle
	if (mIsTriangleCooldown) {
		mTriangleAccumulator += deltaTime;
		if (mTriangleAccumulator >= 1.0f) {
			mTriangleAccumulator = 0.0f;
			mIsTriangleCooldown = false;
		}
	}
	if (mInputState.key[GLFW_KEY_1]&&!mIsTriangleCooldown) {
		float s = distScale(rng);
		float a = glm::radians(distAngleDeg(rng));
		addTriangle(
			glm::vec3(mInputState.mouseX, mInputState.mouseY, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			30.0f,
			0.3f,
			glm::vec3(s, s, 1.0f),
			a
		);
		mIsTriangleCooldown = true;
		
	}
	// Rectangle
	if (mIsRectangleCooldown) {
		mRectangleAccumulator += deltaTime;
		if (mRectangleAccumulator >= 1.0f) {
			mIsRectangleCooldown = false;
			mRectangleAccumulator = 0.0f;
		}
	}
	if (mInputState.key[GLFW_KEY_2]&&!mIsRectangleCooldown) {
		float s = distScale(rng);
		float a = glm::radians(distAngleDeg(rng));
		addRectangle(
			glm::vec3(mInputState.mouseX, mInputState.mouseY, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			30.0f,
			0.3f,
			glm::vec3(s, s, 1.0f),
			a
		);
		mIsRectangleCooldown = true;
	}
	// Circle
	if (mIsCircleCooldown) {
		mCircleAccumulator += deltaTime;
		if (mCircleAccumulator >= 1.0f) {
			mIsCircleCooldown = false;
			mCircleAccumulator = 0.0f;
		}
	}
	if (mInputState.key[GLFW_KEY_3]&&!mIsCircleCooldown) {
		float s = distScale(rng);
		float a = glm::radians(distAngleDeg(rng));
		addCircle(
			glm::vec3(mInputState.mouseX, mInputState.mouseY, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			30.0f,
			0.3f,
			glm::vec3(s, s, 1.0f)
		);
		mIsCircleCooldown = true;
		
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


	for (auto& obj : mObjects) {
		if (obj) obj->draw(*mShader);
	}
	mCup->draw(*mShader);
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

	// Cup
	mCup = new Cup();
	mCup->initialize(this);


	//Object
	sTriangleMesh = createInitTriangle();
	sRectangleMesh = createInitRectangle();
	sCircleMesh = createInitCircle(64, 0.5f);

	/*addTriangle(
		glm::vec3(-0.5f, 0.5f, 0.0f),
		glm::vec3(1.0f,0.0f,0.0f),
		glm::vec3(0.5f,0.5f,1.0f),
		glm::radians(30.0f)
	);

	addRectangle(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.5f, 0.3f, 1.0f),
		glm::radians(45.0f)
	);

	addCircle(
		glm::vec3(0.3f, -0.5f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.3f, 0.3f, 1.0f)
	);*/

	printf("%d", mObjects.size());
}

void Play::unloadData() {
	for (auto& obj : mObjects) {
		obj->setState(Object::DEAD);
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

void Play::moveTo(Parent* parent, Parent::SeqID id) {
	
}