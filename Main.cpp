#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "camera.h"
#include "TextureMesh.h"
#include "ShaderProgram.h"

const unsigned int width = 1200;
const unsigned int height = 1080;

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, "Link's house", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    //get the camera and set up blending
    FirstPersonCamera camera;
    glViewport(0, 0, width, height);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //get a shader program
    ShaderProgram* shaderProg = new ShaderProgram();
    //define projection matrix
    glm::mat4 projection_matrix = glm::perspective(45.0f, width / (float)height, 0.1f, 100.0f);
    double timelasped = 0;
    double last = glfwGetTime();
    double current;
    //Load the opaque texture first
    std::vector<TextureMesh> opaque;
    opaque.push_back(TextureMesh("LinksHouse/Bottles.ply", "LinksHouse/bottles.bmp"));
    opaque.push_back(TextureMesh("LinksHouse/Floor.ply", "LinksHouse/floor.bmp"));
    opaque.push_back(TextureMesh("LinksHouse/DoorBG.ply", "LinksHouse/doorbg.bmp"));
    opaque.push_back(TextureMesh("LinksHouse/WoodObjects.ply", "LinksHouse/woodobjects.bmp"));
    opaque.push_back(TextureMesh("LinksHouse/WindowBG.ply", "LinksHouse/windowbg.bmp"));
    opaque.push_back(TextureMesh("LinksHouse/Walls.ply", "LinksHouse/walls.bmp"));
    opaque.push_back(TextureMesh("LinksHouse/Table.ply", "LinksHouse/table.bmp"));
    //transparent second
    std::vector<TextureMesh> transparent;
    transparent.push_back(TextureMesh("LinksHouse/MetalObjects.ply", "LinksHouse/metalobjects.bmp"));
    transparent.push_back(TextureMesh("LinksHouse/Patio.ply", "LinksHouse/patio.bmp"));
    transparent.push_back(TextureMesh("LinksHouse/Curtains.ply", "LinksHouse/curtains.bmp"));
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        //Get time for camera movement
        current = glfwGetTime();
        timelasped = current - last;
        last = current;
        camera.move(window, timelasped);

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //get the mvp from camera view matrix
        glm::mat4 mvp(1.0);
        mvp = projection_matrix * camera.get_view_matrix();
        //use the shaderProgram and set the time
        glUseProgram(shaderProg->ID);
        shaderProg->setUniform1("time", glfwGetTime());
        //render each mesh within the list
        for (TextureMesh mesh : opaque)
            mesh.draw(mvp, *shaderProg);
        for (TextureMesh mesh : transparent)
            mesh.draw(mvp, *shaderProg);

        glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
}