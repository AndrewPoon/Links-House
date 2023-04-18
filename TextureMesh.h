#ifndef TEXTUREMESH_H
#define TEXTUREMESH_H
#include <glm/glm.hpp>
#include <string>
#include "ShaderProgram.h"
//header only function for texture mesh.
struct TextureMesh {
	unsigned int VBO_vert;
	unsigned int VBO_faces;
	unsigned int textObj;
	unsigned int VAO;
	unsigned int tri_index;
	void draw(glm::mat4 MVP, ShaderProgram& shader);
	TextureMesh(const std::string& plypath, const std::string& bmppath);
};

#endif