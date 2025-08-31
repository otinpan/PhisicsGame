#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "input.h"

enum ShapeType {
	SHAPE_CIRCLE,
	SHAPE_TRIANGLE,
	SHAPE_RECTANGLE,
	SHAPE_POLYGON,
};

class Object {
public:
	enum State
	{
		ACTIVE,
		PAUSED,
		DEAD,
	};

	Object(glm::vec2 center, glm::vec3 rgb);
	~Object();



	virtual void initialize();
	virtual void update(float deltaTime);
	virtual void processInput(const InputState& state);
	virtual void draw();

	// getter
	const glm::vec2& getCenter() { return mCenter; }
	const std::vector<glm::vec3>& getVertices() { return mVertices; }
	const glm::vec2& getVelocity() { return mVelocity; }
	const glm::vec3& getColor() { return mColor; }
	const float getAngle() { return mAngle; }
	class Play* getPlay() { return mPlay; }
	ShapeType getShapeType() { return mShapeType; }
	State getState() const { return mState; }

	// setter
	void setCenter(const glm::vec2& center) { mCenter = center; }
	void setVertices(const std::vector<glm::vec3>& vertices) { mVertices = vertices; }
	void setVelocity(const glm::vec2& velocity) { mVelocity = velocity; }
	void setColor(const glm::vec3& color) { mColor = color; }
	void setAngle(const float angle) { mAngle = angle; }
	void setShapeType(ShapeType type) { mShapeType = type; }


	
private:
	class Play* mPlay;

	State mState;

	ShapeType mShapeType;

	glm::vec2 mCenter; // 中心座標
	std::vector<glm::vec3> mVertices; // 頂点座標

	glm::vec2 mVelocity; //速度
	float mAngle; // 角度

	glm::vec3 mColor;
	
};