#pragma once
#define INCLUDE_SEQUENCE_PARENT_H

#include <array> 
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "input.h"


class Parent {
public:
	enum SeqID {
		SEQ_PLAY,
		SEQ_NONE,
	};
	
	Parent(GLFWwindow* w);
	~Parent();
	
	void update();

	void gatherInput(InputState& state);
	
	void moveTo(SeqID next, SeqID pre);
	
	class Play*& GetPlay() { return mPlay; }
	
	SeqID GetNextSeq() { return mNext; }
	SeqID GetPreSeq() { return mPre; }
	
	
private:
	class Play* mPlay;
	
	SeqID mNext;
	SeqID mPre;

	GLFWwindow* mWindow=nullptr;
	
	void safeDelete();
};