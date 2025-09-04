#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "input.h"
#include "shader.h"

class Cup {
public:
	Cup();
	~Cup();

	void initialize(class Play* play);

	void update(float deltaTime);
	void processInput(const InputState& state);
	void draw(Shader &shader);

	// getter
	const glm::vec3& getCenter() { return mCenter; }
	const float getWidth() { return mWidth; }
	const float getHeight() { return mHeight; }
	const glm::vec3& getColor() { return mColor; }
	const float getLeft() { return mLeft; }
	const float getRight() { return mRight; }
	const float getTop() { return mTop; }
	const float getBottom() { return mBottom; }
	const float getRestitution() { return mRestitution; }

	// setter
	void setCenter(const glm::vec3& center) { mCenter = center; }
	void setWidth(const float width) { mWidth = width; }
	void setHeight(const float height) { mHeight = height; }
	void setColor(const glm::vec3& color) { mColor = color; }
	void setLeft(const float left) { mLeft = left; }
	void setRight(const float right) { mRight = right; }
	void setTop(const float top) { mTop = top; }
	void setBottom(const float bottom) { mBottom = bottom; }
	

private:
	glm::vec3 mCenter;
	float mWidth;
	float mHeight;
	glm::vec3 mColor;
	float mLineWidth;

	// 反発係数
	float mRestitution;

	// 端っこの座標
	float mLeft;
	float mRight;
	float mTop;
	float mBottom;

	// 奥の頂点
	float mBackLeft;
	float mBackRight;
	float mBackTop;
	

	std::vector<glm::vec3> mVertices;

	// OpenGL関連
	unsigned int VAO, VBO;
	glm::mat4 mModel;
	
};