#include <gl/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstddef>
#include "datastruct.h"
#include "ShaderProgram.h"
#include "TextureMesh.h"

//split the string on spaces and return as a list.
std::vector<std::string> split(const std::string& input, char space) {
	std::vector<std::string> splited;

	std::string temp;
	std::stringstream stream(input);
	while (getline(stream, temp, space))
		splited.push_back(temp);
	return splited;
}
//get file name and open it
void readPLYFile(const std::string& filename, std::vector<VertexData>& vertices, std::vector<TriData>& faces) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cout << "Cannot Open file " << filename << std::endl;
		return;
	}

	int vertexLines = 0, faceLines = 0;
	std::vector < std::string > order;

	std::string line;
	//while reading each line of file
	while (std::getline(file, line)) {
		//get the # of vertex
		if (line.find("element vertex") != std::string::npos) {
			vertexLines = std::stoi(split(line, ' ')[2]);
		}
		//get the # of faces
		if (line.find("element face") != std::string::npos) {
			faceLines = std::stoi(split(line, ' ')[2]);
		}
		//get the order of the vertices.
		if (line.find("property float") != std::string::npos) {
			order.push_back(split(line, ' ').back());
		}
		if (line == "end_header") break;
	}
	int i = 0;
	//read the eachline after end header and save the vertex read as a Vertexdaa Object
	while (i < vertexLines && std::getline(file, line)) {
		std::vector<std::string>verts = split(line, ' ');
		if (verts.size() != order.size()) {
			std::cout << "readPLYFile: Error parsing " << filename << std::endl;
			return;
		}

		VertexData vertex(0, 0, 0);
		for (int j = 0; j < verts.size(); j++) {
			if (order[j] == "x")
				vertex.pos.x = std::stof(verts[j]);
			if (order[j] == "y")
				vertex.pos.y = std::stof(verts[j]);
			if (order[j] == "z")
				vertex.pos.z = std::stof(verts[j]);
			if (order[j] == "nx")
				vertex.normal.x = std::stof(verts[j]);
			if (order[j] == "ny")
				vertex.normal.y = std::stof(verts[j]);
			if (order[j] == "nz")
				vertex.normal.z = std::stof(verts[j]);
			if (order[j] == "red")
				vertex.color.r = std::stof(verts[j]);
			if (order[j] == "green")
				vertex.color.g = std::stof(verts[j]);
			if (order[j] == "blue")
				vertex.color.b = std::stof(verts[j]);
			if (order[j] == "u")
				vertex.text.x = std::stof(verts[j]);
			if (order[j] == "v")
				vertex.text.y = std::stof(verts[j]);
		}
		vertices.push_back(vertex);
		i++;
	}
	//same here for face indices.
	i = 0;
	while (i < faceLines && std::getline(file, line)) {
		std::vector<std::string> indices = split(line, ' ');

		TriData face(std::stoi(indices[1]), std::stoi(indices[2]), std::stoi(indices[3]));
		faces.push_back(face);

		i++;
	}

	file.close();

};

void loadARGB_BMP(const char* imagepath, unsigned char** data, unsigned int* width, unsigned int* height) {

	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGBA data

	// Open the file
	FILE* file;
	errno_t err = fopen_s(&file, imagepath, "rb");
	if (!file) {
		printf("%s could not be opened. Are you in the right directory?\n", imagepath);
		getchar();
		return;
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file1\n");
		fclose(file);
		return;
	}

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	*width = *(int*)&(header[0x12]);
	*height = *(int*)&(header[0x16]);
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file2\n");
		fclose(file);
		return;
	}
	// Make sure this is a 32bpp file
	if (*(int*)&(header[0x1E]) != 3) {
		printf("Not a correct BMP file3\n");
		fclose(file);
		return;
	}
	// fprintf(stderr, "header[0x1c]: %d\n", *(int*)&(header[0x1c]));
	// if ( *(int*)&(header[0x1C])!=32 ) {
	//     printf("Not a correct BMP file4\n");
	//     fclose(file);
	//     return;
	// }

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = (*width) * (*height) * 4; // 4 : one byte for each Red, Green, Blue, Alpha component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	*data = new unsigned char[imageSize];

	if (dataPos != 54) {
		fread(header, 1, dataPos - 54, file);
	}

	// Read the actual data from the file into the buffer
	fread(*data, 1, imageSize, file);

	// Everything is in memory now, the file can be closed.
	fclose(file);
};

TextureMesh::TextureMesh(const std::string& plypath, const std::string& bmppath) {
	std::vector<VertexData> vertices;
	std::vector<TriData> faces;
	//modify vertices and faces with data
	readPLYFile(plypath, vertices, faces);

	//Define our VAO and bind
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Define our VBO buffer for vertices, bind and fill
	glGenBuffers(1, &VBO_vert);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vert);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexData),
		&vertices[0], GL_STATIC_DRAW);

	//enable our vertex attrbiute pointer for vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 
		(void*)offsetof(VertexData, VertexData::pos));
	//For texture
	glEnableVertexAttribArray(1);  
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
		(void*)offsetof(VertexData, VertexData::text));

	//Same for the face
	glGenBuffers(1, &VBO_faces);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_faces);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(TriData),
		&faces[0], GL_STATIC_DRAW);
	tri_index = faces.size() * 3; 

	//load bmp and store the rgb values
	unsigned char* imageData;
	unsigned int img_width, img_height;
	loadARGB_BMP(bmppath.c_str(), &imageData, &img_width, &img_height);

	//create texture object and bind
	glGenTextures(1, &textObj);
	glBindTexture(GL_TEXTURE_2D, textObj);
	//load the image data 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	//unbind VAO and texture
	glBindVertexArray(0); 
	glBindTexture(GL_TEXTURE_2D, 0);
};

//get the shader ID and MVP and draw the meshes
void TextureMesh::draw(glm::mat4 MVP, ShaderProgram& shader) {
	//bind the VAO and use the shader program to set MVP
	glBindVertexArray(VAO);
	glUseProgram(shader.ID);
	shader.setUniformMatrix4fv("MVP", MVP);

	//use the texture object and set the tex variable within shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textObj);
	shader.setUniform1("tex", 0); 

	//draw the meshes
	glDrawElements(GL_TRIANGLES, tri_index, GL_UNSIGNED_INT, 0);

	// Unbind 
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
};