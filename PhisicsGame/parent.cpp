#include "parent.h"
#include "play.h"


Parent::Parent(GLFWwindow* w)
	:mPlay(0)
	,mNext(SEQ_NONE)
	,mPre(SEQ_NONE)
	,mWindow(w)
{
	mPlay = new Play();
}

Parent::~Parent() {
	if (mPlay) {
		delete mPlay;
		mPlay = nullptr;
	}
}

void Parent::gatherInput(InputState& state) {
	if (!mWindow) {
		printf("No window\n");
		throw;
	}
	state.key[GLFW_KEY_W] = glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS;
	state.key[GLFW_KEY_A] = glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS;
	state.key[GLFW_KEY_S] = glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS;
	state.key[GLFW_KEY_D] = glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS;
	state.key[GLFW_KEY_1] = glfwGetKey(mWindow, GLFW_KEY_1) == GLFW_PRESS;
	state.key[GLFW_KEY_2] = glfwGetKey(mWindow, GLFW_KEY_2) == GLFW_PRESS;
	state.key[GLFW_KEY_3] = glfwGetKey(mWindow, GLFW_KEY_3) == GLFW_PRESS;
	
	double x, y;
	glfwGetCursorPos(mWindow, &x, &y);
	state.dx = static_cast<float>(x - state.mouseX);
	state.dy = static_cast<float>(y - state.mouseY);
	unsigned int mScreenWidth, mScreenHeight;
	glfwGetFramebufferSize(mWindow, (int*)&mScreenWidth, (int*)&mScreenHeight);
	x = std::max((double) 0, (double)x);
	y = std::max((double)0, (double)y);
	x = std::min((double)mScreenWidth, (double)x);
	y = std::min((double)mScreenHeight, (double)y);

	// worldç¿ïWÇ…ïœä∑
	x *= (float)2.0f / (float)mScreenWidth;
	x -= 1.0f;
	y *= -(float)2.0f / (float)mScreenHeight;
	y += 1.0f;

	state.mouseX = static_cast<float>(x);
	state.mouseY = static_cast<float>(y);
}

void Parent::update() {
	switch (mNext) {
	case SEQ_PLAY:
		safeDelete();
		mPlay = new Play();
	}

	mNext = SEQ_NONE;

	if (mPlay)mPlay->update(this);
}

void Parent::moveTo(SeqID next, SeqID pre) {
	mNext = next;
	mPre = pre;
}

void Parent::safeDelete() {
	if (mPlay) {
		delete mPlay;
		mPlay = nullptr;
	}
}