#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>

//shader program class that contains the program ID with fragment and vertex shader loaded. 
class ShaderProgram {
public:
    unsigned int ID;

    ShaderProgram() {
        ID = createShaderProg();
    }
    //set uniform variables(tex,time,MVP) with the shader source
    void setUniform1(const std::string& name, float val) {
        glProgramUniform1i(ID, getLocation(name), val);
    }
    void setUniformMatrix4fv(const std::string& name, glm::mat4 val) {
        glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(val));
    }
    GLint getLocation(const std::string& name) {
        return glGetUniformLocation(ID, name.c_str());
    }
    //create and load shader program
    unsigned int createShaderProg() {
        //Vertex shader code
        std::string VertexShaderCode = "\
    	#version 330 core\n\
		// Input vertex data, different for all executions of this shader.\n\
		layout(location = 0) in vec3 vertexPosition;\n\
		layout(location = 1) in vec2 uv;\n\
		// Output data ; will be interpolated for each fragment.\n\
		out vec2 uv_out;\n\
		// Values that stay constant for the whole mesh.\n\
		uniform mat4 MVP;\n\
		uniform float time; \n\
        void main(){ \n\
			// Output position of the vertex, in clip space : MVP * position\n\
			gl_Position =  MVP * vec4(vertexPosition,1);\n\
			// The color will be interpolated to produce the color of each fragment\n\
			uv_out = uv;\n\
		}\n";

        // Fragment Shader code 
        std::string FragmentShaderCode = "\
		#version 330 core\n\
        out vec4 fragCol;\n\
		in vec2 uv_out; \n\
		uniform sampler2D tex;\n\
		void main() {\n\
			fragCol = texture(tex, uv_out);\n\
		}\n";
        //create ID for vertex/frag shader and load source code into it
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        const char* vertex_code_pointer = VertexShaderCode.c_str();
        const char* frag_code_pointer = FragmentShaderCode.c_str();
        glShaderSource(VertexShaderID, 1, &vertex_code_pointer, NULL);
        glShaderSource(FragmentShaderID, 1, &frag_code_pointer, NULL);


        //Compile both of the shader
        glCompileShader(VertexShaderID);
        glCompileShader(FragmentShaderID);

        //create the program and attach both shader onto it
        unsigned int prog = glCreateProgram();
        glAttachShader(prog, VertexShaderID);
        glAttachShader(prog, FragmentShaderID);
        glLinkProgram(prog);

        //delete all of the shader and return the program ID with shaders
        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);
        return prog;
    }
};
#endif