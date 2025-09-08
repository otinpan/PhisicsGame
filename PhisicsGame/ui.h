#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <chrono>
#include <random>

class UI {
public:
	UI();
	~UI();

	void initialize(class Play* play);
	void update(float deltaTime);
	void draw(class Shader& shader);

	void takeUIObject(); // objectÇéÊÇËèoÇ∑

	void addUIObject(glm::vec3 pos); // mUIObjectÇ…í«â¡

private:
	class Play* mPlay;

	std::vector<class UIObject*> mUIObjects;

	glm::vec3 mUIObjectBottomPos;

	// random
	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> distScale{ 0.1f, 0.3f };
	std::uniform_int_distribution<int> distShapeType{ 0,2 };
};