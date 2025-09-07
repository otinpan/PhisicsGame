#pragma once
#include "GLMesh.h"
#include <vector>
#include "input.h"
#include "shader.h"

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

	Object(glm::vec3 center, glm::vec3 rgb,GLMesh &mesh,float mass,float restitution,
		glm::vec3 scale=glm::vec3(1.0f,1.0f,1.0f),float angle=0.0f);
	~Object();

	virtual void initialize(class Play* play);
	virtual void update(float deltaTime);
	virtual void processInput(const InputState& state);
	virtual void draw(Shader& shader);

	// getter
	const glm::vec3& getCenter() { return mCenter; }
	const std::vector<glm::vec3>& getVertices() { return mVertices; }
	const std::vector<glm::vec3>& getSVertices() { return sVertices; }
	const glm::vec2& getVelocity() { return mVelocity; }
	const float getRadius() { return mRadius; }
	const float getMass() { return mMass; }
	const float getRestitution() { return mRestitution; }
	const float getInertia() { return mInertia; }
	const float getAngularSpeed() { return mAngularSpeed; }
	const float getAngularAcceleration() { return mAngularAcceleration; }
	const glm::vec3& getColor() { return mColor; }
	const float getAngle() { return mAngle; }
	const glm::vec3& getScale() { return mScale; }
    const glm::mat4& getModel() { return mModel; }
	class Play* getPlay() { return mPlay; }
	const GLMesh& getMesh() { return mMesh; }
	ShapeType getShapeType() { return mShapeType; }
	State getState() const { return mState; }
	const float getHitRadius() { return mHitRadius; }

	// setter
	void setCenter(const glm::vec3& center) { mCenter = center; }
	void setVertices(const std::vector<glm::vec3>& vertices) { mVertices = vertices; }
	void setSVertices(const std::vector<glm::vec3>& vertices) { sVertices = vertices; }
	void setVelocity(const glm::vec2& velocity) { mVelocity = velocity; }
	void setMass(const float mass) { mMass = mass; }
	void setRestitution(const float restitution) { mRestitution = restitution; }
	void setInertia(const float inertia) { mInertia = inertia; }
	void setAngularSpeed(const float angularSpeed) { mAngularSpeed = angularSpeed; }
	void setAngularAcceleration(const float angularAcceleration) { mAngularAcceleration = angularAcceleration; }
	void setRadius(const float radius) { mRadius = radius; }
	void setColor(const glm::vec3& color) { mColor = color; }
	void setAngle(const float angle) { mAngle = angle; }
	void setScale(const glm::vec3& scale) { mScale = scale; }
	void setModel(const glm::mat4& model) { mModel = model; }
	void setMesh(const GLMesh& mesh) { mMesh = mesh; }
	void setShapeType(ShapeType type) { mShapeType = type; }
	void setState(State state) { mState = state; }
	void setHitRadius(const float hitRadius) { mHitRadius = hitRadius; }



	
private:
	class Play* mPlay;

	State mState;

	ShapeType mShapeType;

	glm::vec3 mCenter; // ���S���W
	std::vector<glm::vec3> mVertices; // ���_���W
	std::vector<glm::vec3> sVertices; // �ŏ��̒��_���W

	// �^��
	glm::vec2 mVelocity; //���x
	float mMass; // ����
	float mRestitution; // �����W��
	glm::vec2 mGravity; // �d��
	float mInertia; // �������[�����g
	float mAngularSpeed; //�p���x
	float mAngularAcceleration; //�p�����x

	float mAngle; // �p�x
	glm::vec3 mScale; // �X�P�[��
	float mRadius; // ���a(�~�̂�)

	glm::vec3 mColor; // �F

	// model�s��
	glm::mat4 mModel;

	GLMesh mMesh; //��ƂȂ郁�b�V��

	// hitbox
	float mHitRadius;

protected:
	virtual void collisionCup();
	virtual void solveCollisionCup(glm::vec2 normal, glm::vec2 contactPoint);
	
	
};

inline float cross2D(const glm::vec2& a, const glm::vec2& b) {
	return a.x * b.y - a.y * b.x;
}