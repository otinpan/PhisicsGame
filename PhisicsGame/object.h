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
	const glm::vec3& getColor() { return mColor; }
	const float getAngle() { return mAngle; }
	const glm::vec3& getScale() { return mScale; }
    const glm::mat4& getModel() { return mModel; }
	class Play* getPlay() { return mPlay; }
	const GLMesh& getMesh() { return mMesh; }
	ShapeType getShapeType() { return mShapeType; }
	State getState() const { return mState; }

	// setter
	void setCenter(const glm::vec3& center) { mCenter = center; }
	void setVertices(const std::vector<glm::vec3>& vertices) { mVertices = vertices; }
	void setSVertices(const std::vector<glm::vec3>& vertices) { sVertices = vertices; }
	void setVelocity(const glm::vec2& velocity) { mVelocity = velocity; }
	void setRadius(const float radius) { mRadius = radius; }
	void setColor(const glm::vec3& color) { mColor = color; }
	void setAngle(const float angle) { mAngle = angle; }
	void setScale(const glm::vec3& scale) { mScale = scale; }
	void setModel(const glm::mat4& model) { mModel = model; }
	void setMesh(const GLMesh& mesh) { mMesh = mesh; }
	void setShapeType(ShapeType type) { mShapeType = type; }
	void setState(State state) { mState = state; }



	
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

	float mAngle; // �p�x
	glm::vec3 mScale; // �X�P�[��
	float mRadius; // ���a(�~�̂�)

	glm::vec3 mColor; // �F

	// model�s��
	glm::mat4 mModel;

	GLMesh mMesh; //��ƂȂ郁�b�V��
	
};

bool isIntersects(Object* A, Object* B);
bool isIntersectsCC(Object* c1, Object* c2);
bool isIntersectsCP(Object* c, Object* p);
bool isIntersectsPP(Object* p1, Object* p2);

void solveCollisionCC(Object* c1, Object* c2);