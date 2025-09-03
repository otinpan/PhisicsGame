#pragma once
#include <glm/glm.hpp>
#include <vector>

struct GLMesh {
	std::vector<glm::vec3> vertices;
	unsigned int VAO=0;
	unsigned int VBO=0;
	unsigned int EBO=0;
	size_t vertexCount=0; // ’¸“_‚Ì”
	size_t indexCount=0; // index‚Ì”
	float radius = 0.0f; // ‰~‚Ìê‡
};

GLMesh createInitTriangle();
GLMesh createInitRectangle();
GLMesh createInitCircle(unsigned int segments = 32, float radius = 0.5f);
void destroyMesh(GLMesh& m);

