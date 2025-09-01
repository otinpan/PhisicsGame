#pragma once
#include "shader.h"
#include "GLMesh.h"
#include "parent.h"
#include <vector>
#include <chrono>


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
	void processInput(Parent* parent);
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

	// çXêV
	static constexpr float deltaTime = 1.0f / 60.0f;
	std::chrono::steady_clock::time_point mLastTime{};
	float mAccumulator = 0.0f;

	// ï`âÊMeshÇÃèâä˙âª
	GLMesh sTriangleMesh;
	GLMesh sRectangleMesh;
	GLMesh sCircleMesh;

	// Shader
	class Shader* mShader;

	


};
