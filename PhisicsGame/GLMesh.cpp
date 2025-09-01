#include <glad/glad.h>
#include "GLMesh.h"


GLMesh createInitTriangle() {
	float vertices[] = {
		-0.5f,-0.2887f,0.0f,
		0.5f,-0.2887f,0.0f,
		0.0f,0.5774f,0.0f,
	};

	GLMesh m;
	m.vertices = {
		glm::vec3(-0.5f, -0.2887f, 0.0f),
		glm::vec3(0.5f, -0.2887f, 0.0f),
		glm::vec3(0.0f, 0.5774f, 0.0f)
	};
	m.vertexCount = 3;


	glGenVertexArrays(1, &m.VAO); // VAO�𐶐�  VAO=1�̂悤�Ɏ��ʎq���U����
	glGenBuffers(1, &m.VBO);

	glBindVertexArray(m.VAO);

	// VBO��gpu�ɑ��M
	glBindBuffer(GL_ARRAY_BUFFER, m.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// shader�ƒ��_�������т���
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// ����
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return m;
}

GLMesh createInitRectangle() {
	float vertices[] = {
		0.5f, 0.5f, 0.0f,  // �E��
		0.5f, -0.5f, 0.0f, // �E��
		-0.5f, -0.5f, 0.0f,// ����
		-0.5f, 0.5f, 0.0f  // ����
	};

	unsigned int indices[] = {
		0, 1, 3, // ��1�O�p�`
		1, 2, 3  // ��2�O�p�`
	};


	GLMesh m;
	m.vertices = {
		glm::vec3(0.5f, 0.5f, 0.0f),  // �E��
		glm::vec3(0.5f, -0.5f, 0.0f), // �E��
		glm::vec3(-0.5f, -0.5f, 0.0f),// ����
		glm::vec3(-0.5f, 0.5f, 0.0f)  // ����
	};
	m.vertexCount = 4;
	m.indexCount = 6;

	glGenVertexArrays(1, &m.VAO);
	glGenBuffers(1, &m.VBO);
	glGenBuffers(1, &m.EBO);

	glBindVertexArray(m.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// ����
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return m;
}

void destroyMesh(GLMesh& m) {
	if (m.EBO)glDeleteBuffers(1, &m.EBO);
	if (m.VBO)glDeleteBuffers(1, &m.VBO);
	if (m.VAO)glDeleteVertexArrays(1, &m.VAO);
	m = {};
}