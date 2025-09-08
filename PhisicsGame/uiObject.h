#pragma once
#include "GLMesh.h"
#include "shader.h"

class UIObject {
public:
	enum ShapeType {
		SHAPE_CIRCLE,
		SHAPE_TRIANGLE,
		SHAPE_RECTANGLE,
	};

	UIObject(glm::vec3 pos,glm::vec3 rgb,GLMesh& mesh,float mass,
		float restitution,glm::vec3 scale);
	~UIObject();

	void initialize(class Play* play);
	void update(float deltaTime);
	void draw(Shader& shader);

	// getter
	const glm::vec3& getPosition() { return mPosition; }
	const glm::vec3& getColor() { return mColor; }
	const float getMass() { return mMass; }
	const float getRestitution() { return mRestitution; }
	const glm::vec3& getScale() { return mScale; }
	const glm::mat4& getModel() { return mModel; }
	const GLMesh& getMesh() { return mMesh; }
	ShapeType getShapeType() { return mShapeType; }

	// setter
	void setPosition(const glm::vec3& pos) { mPosition = pos; }
	void setColor(const glm::vec3& color) { mColor = color; }
	void setMass(const float mass) { mMass = mass; }
	void setRestitution(const float restitution) { mRestitution = restitution; }
	void setScale(const glm::vec3& scale) { mScale = scale; }
	void setModel(const glm::mat4& model) { mModel = model; }
	void setMesh(const GLMesh& mesh) { mMesh = mesh; }
	void setShapeType(ShapeType type) { mShapeType = type; }

private:
	GLMesh mMesh;
	class Play* mPlay;

	ShapeType mShapeType;

	glm::vec3 mPosition;
	glm::vec3 mColor;
	float mMass;
	float mRestitution;
	glm::vec3 mScale;

	glm::mat4 mModel;

	std::vector<glm::vec3> sVertices;
};