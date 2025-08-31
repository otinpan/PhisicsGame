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
	
	double x, y;
	glfwGetCursorPos(mWindow, &x, &y);
	state.dx = static_cast<float>(x - state.mouseX);
	state.dy = static_cast<float>(y - state.mouseY);
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