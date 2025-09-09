#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <chrono>
#include <random>

struct objectData {
	glm::vec3 color;
	glm::vec3 scale;
	unsigned int shapetype;
};

class UI {
public:
	UI();
	~UI();

	void initialize(class Play* play);
	void update(float deltaTime);
	void drawUIObject(class Shader& shader);

	objectData takeUIObject(); // object�����o��

	void createUIObject(glm::vec3 pos); // UIObject���쐬

	void addUIObject(class UIObject* obj); // mUIObject�ɒǉ�
	void removeUIObject(class UIObject* obj); // mUIObject����폜

private:
	class Play* mPlay;

	std::vector<class UIObject*> mUIObjects;

	glm::vec3 mUIObjectBottomPos;
	glm::vec3 mUIObjectMiddlePos;
	glm::vec3 mUIObjectTopPos;

	// random
	std::mt19937 rng{ std::random_device{}() };
	std::mt19937 rng2{ std::random_device{}() };
	std::uniform_real_distribution<float> distScale{ 0.1f, 0.25f };
	std::uniform_int_distribution<int> distShapeType{ 0,2 };
};