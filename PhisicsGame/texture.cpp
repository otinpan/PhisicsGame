#include<glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include "texture.h"
#include "shader.h"

Texture::Texture(glm::vec3 pos,glm::vec3 scale,GLMesh &mesh,float angle)
	:mPosition(pos)
	,mScale(scale)
	, mMesh(mesh)
	, mAngle(angle)
{
	mModel = glm::mat4(1.0f);
}

Texture::~Texture() {

}

void Texture::initialize(const char* filePath) {

	glm::mat4 model(1.0f);
	model = glm::translate(model, mPosition);
	model = glm::rotate(model, mAngle, glm::vec3(0, 0, 1));
	model = glm::scale(model, mScale);
	mModel = model;

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

	if (data) {
		GLenum format = GL_RGB;
		if (nrChannels == 1) format = GL_RED;
		else if (nrChannels == 3) format = GL_RGB;
		else if (nrChannels == 4) format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
			GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "Failed to load texture: " << filePath << std::endl;
	}
	stbi_image_free(data);
}

void Texture::setPosition(const glm::vec3& pos) {
	mPosition = pos;
	glm::mat4 model(1.0f);
	model = glm::translate(model, mPosition);
	model = glm::rotate(model, mAngle, glm::vec3(0, 0, 1));
	model = glm::scale(model, mScale);
	mModel = model;
}

void Texture::setScale(const glm::vec3& scale) {
	mScale = scale;
	glm::mat4 model(1.0f);
	model = glm::translate(model, mPosition);
	model = glm::rotate(model, mAngle, glm::vec3(0, 0, 1));
	model = glm::scale(model, mScale);
	mModel = model;
}

void Texture::draw(Shader& shader) {
	shader.use();
	shader.setInt("texture1", 0);
	shader.setMatrix4("model",mModel);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glBindVertexArray(mMesh.VAO);
	glDrawElements(GL_TRIANGLES, mMesh.indexCount, GL_UNSIGNED_INT, 0);
}