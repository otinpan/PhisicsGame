#pragma once
#include <array> 
#include <GLFW/glfw3.h>

struct InputState {
	std::array<unsigned char, GLFW_KEY_LAST + 1> key{};
	float mouseX = 0.0f, mouseY = 0.0f, dx = 0.0f, dy = 0.0f;
	// ¶ƒNƒŠƒbƒN
	bool mouseLeftDown = false;
	bool mouseLeftPressed = false;
	bool mouseLeftReleased = false;
	bool prevMouseLeftDown = false;


	bool down(int k) const { return k >= 0 && k <= GLFW_KEY_LAST && key[k]; }
};