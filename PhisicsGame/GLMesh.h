#pragma once
#include <glm/glm.hpp>
#include <vector>

struct GLMesh {
	std::vector<glm::vec3> vertices;
	unsigned int VAO=0;
	unsigned int VBO=0;
	unsigned int EBO=0;
	size_t vertexCount=0; // ���_�̐�
	size_t indexCount=0; // index�̐�
};

GLMesh createInitTriangle();
GLMesh createInitRectangle();
void destroyMesh(GLMesh& m);

