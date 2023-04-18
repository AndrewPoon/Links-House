#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include<glm/glm.hpp>
// data structure for vertex data and tri data as specified.
class VertexData {
public:
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 text;
	VertexData(float x, float y, float z, float nx = 0.0f, float ny = 0.0f, float nz = 0.0f, float r = 0.0f, float g = 0.0f, float b = 0.0f, float u = 0.0f, float v = 0.0f) {
		pos = glm::vec3(x, y, z);
		normal = glm::vec3(nx, ny, nz);
		color = glm::vec3(r, g, b);
		text = glm::vec2(u, v);
	}
};
class TriData {
public:
	int v1, v2, v3;
	TriData(int index_1, int index_2, int index_3) {
		v1 = index_1;
		v2 = index_2;
		v3 = index_3;

	}

};

#endif