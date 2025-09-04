#pragma once
#include "shader.h"
#include "GLMesh.h"
#include "parent.h"
#include <vector>
#include <chrono>
#include <random>

class Play {
public:
	Play();
	~Play();

	void update(class Parent*);
	void draw();

	bool initialize();
	void shutdown();

	void addObject(class Object* obj);
	void removeObject(class Object* obj);

	// hitstop
	void setIsHitstop(bool isHitstop) { mIsHitstop = isHitstop; }
	bool getIsHitstop() { return mIsHitstop; }
	void setHitstopTime(float time);
	void updateHitstop(float deltaTime);

	// getter
	const std::vector<class Object*>& getObjects() { return mObjects; }
	class Cup* getCup() { return mCup; }

	// sequence
	void moveTo(Parent* parent, Parent::SeqID id);
	void setSeqID(Parent::SeqID id) { mSeqID = id; }

private:
	void processInput(Parent* parent,float deltaTime);
	void updatePlay(float deltaTime);
	void loadData();
	void unloadData();

	class Cup* mCup;
	std::vector<class Object*> mObjects;
	std::vector<class Object*> mPendingObjects;

	bool mIsRunning;
	bool mUpdatingObjects;

	bool mIsHitstop;
	float hitstopTime;
	float mHitstopTime;

	Parent::SeqID mSeqID;

	InputState mInputState;

	// çXêV
	static constexpr float deltaTime = 1.0f / 60.0f;
	std::chrono::steady_clock::time_point mLastTime{};
	float mAccumulator = 0.0f;

	// ï`âÊMeshÇÃèâä˙âª
	GLMesh sTriangleMesh;
	GLMesh sRectangleMesh;
	GLMesh sCircleMesh;

	// Objectí«â¡
	void addTriangle(glm::vec3 center, glm::vec3 color, float mass=30,float restitution=0.2f, glm::vec3 scale = { 1.0f,1.0f,1.0f }, float angle=0.0f);
	void addRectangle(glm::vec3 center, glm::vec3 color, float mass = 30, float restitution = 0.2f, glm::vec3 scale = { 1.0f,1.0f,1.0f }, float angle=0.0f);
	void addCircle(glm::vec3 center, glm::vec3 color, float mass = 30, float restitution = 0.2f, glm::vec3 scale = { 1.0f,1.0f,1.0f }, float angle=0.0f);
	bool mIsTriangleCooldown;
	bool mIsRectangleCooldown;
	bool mIsCircleCooldown;
	float mTriangleAccumulator;
	float mRectangleAccumulator;
	float mCircleAccumulator;

	// Shader
	class Shader* mShader;

	// random
	// Play.h
	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> distScale{ 0.1f, 0.3f };
	std::uniform_real_distribution<float> distAngleDeg{ 0.0f, 360.0f };

};
