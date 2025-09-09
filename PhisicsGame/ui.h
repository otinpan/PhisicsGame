#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <chrono>
#include <random>
#include "GLMesh.h"

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
	void drawTexture(class Shader& shader);
	
	// end game
	void endGame();

	objectData takeUIObject(); // object‚ğæ‚èo‚·

	void createUIObject(glm::vec3 pos); // UIObject‚ğì¬

	void addUIObject(class UIObject* obj); // mUIObject‚É’Ç‰Á
	void removeUIObject(class UIObject* obj); // mUIObject‚©‚çíœ

private:
	class Play* mPlay;


	std::vector<class UIObject*> mUIObjects;
	class UIObject* mEndObject;

	glm::vec3 mUIObjectBottomPos;
	glm::vec3 mUIObjectMiddlePos;
	glm::vec3 mUIObjectTopPos;

	// texure
	std::vector<class Texture*> mTextures;
	std::vector<class Texture*> numTextures;
	std::vector<class Texture*> mScoreTextures;
	std::vector<class Texture*> mEndTextures;

	// mesh
	GLMesh sTextureMesh;

	// counter
	int mScore = 0;

	// random
	std::mt19937 rng{ std::random_device{}() };
	std::mt19937 rng2{ std::random_device{}() };
	std::uniform_real_distribution<float> distScale{ 0.15f, 0.25f };
	std::uniform_int_distribution<int> distShapeType{ 0,2 };
};