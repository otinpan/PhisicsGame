#pragma once
#include "GLMesh.h"

class Texture {
public:
	Texture(glm::vec3 pos, glm::vec3 scale, GLMesh& mesh,float angle=0.0f);
	~Texture();

	void initialize(const char* texturePath);
	void setPosition(const glm::vec3& pos);
	void setScale(const glm::vec3& scale);


	void draw(class Shader& shader);
private:
	GLMesh mMesh;

	glm::vec3 mPosition;
	glm::vec3 mScale;
	float mAngle;


	unsigned int mTextureID;

	glm::mat4 mModel;
};