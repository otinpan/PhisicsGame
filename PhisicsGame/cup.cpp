#include <glad/glad.h>
#include "play.h"
#include "cup.h"


Cup::Cup()
	: mLeft(-0.5f)
	, mRight(0.5f)
	, mTop(0.3f)
	, mBottom(-0.9f)
	, mColor(glm::vec3(1.0f, 1.0f, 0.3f))
	, mLineWidth(5.0f)
{
	mCenter = glm::vec3((mLeft + mRight) / 2.0f, (mTop + mBottom) / 2.0f, 0.0f);
	mWidth = mRight - mLeft;
	mHeight = mTop - mBottom;

	mBackLeft = mLeft + 0.1f;
	mBackRight = mRight - 0.1f;
	mBackTop = mTop + 0.1f;

	mModel = glm::mat4(1.0f);
}

Cup::~Cup() {

}

void Cup::initialize(class Play* play) {
	float vertices[] = {
		mLeft,mTop,0.0f,    // 左上
		mLeft,mBottom,0.0f, // 左下
		mRight,mBottom,0.0f,// 右下
		mRight,mTop,0.0f,    // 右上
		mLeft,mTop,0.0f,	// 左上
		mBackLeft,mBackTop,0.0f, // 左上(奥)
		mBackRight,mBackTop,0.0f, // 右上(奥)
		mRight,mTop,0.0f,	// 右上
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Cup::update(float deltaTime) {

}

void Cup::processInput(const InputState& state) {
	
}

void Cup::draw(Shader &shader) {
	shader.use();
	shader.setMatrix4("model", mModel);
	shader.setVec3("objectColor", mColor);

	glBindVertexArray(VAO);
	glLineWidth(mLineWidth);
	glDrawArrays(GL_LINE_STRIP, 0, 8);
}