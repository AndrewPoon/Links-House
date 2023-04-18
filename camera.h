#ifndef CAMERA_H
#define CAMERA_H
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Camera class 
class FirstPersonCamera {
public:
    //variables needed for camera
    glm::vec3 camera_pos;
    glm::vec3 camera_dir;
    glm::vec3 camera_up;
    float camera_speed;
    float camera_rotation_speed;
    float camera_angle;
    FirstPersonCamera() {
        // Set up the camera
        camera_pos = glm::vec3(0.5f, 0.4f, 0.5f);
        camera_dir = glm::vec3(0.0f, 0.0f, -1.0f);
        camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
        camera_speed = 5.0f;
        camera_rotation_speed = 60.0f;
        camera_angle = 90.0f;
    };
    //return view matrix for MVP
    glm::mat4 get_view_matrix() {
        return glm::lookAt(camera_pos, camera_pos + camera_dir, camera_up);
    };
    //move the camera variables via key presses and time lasped
    void move(GLFWwindow* window, float time_pressed) {
        float distance = camera_speed * time_pressed;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            camera_pos += distance * camera_dir;
        else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            camera_pos -= distance * camera_dir;
        else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            camera_angle += camera_rotation_speed * time_pressed;
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            camera_angle -= camera_rotation_speed * time_pressed;
        //camera is locked yaw motion so only x and z needed to be change.
        camera_dir.x = sin(glm::radians(camera_angle));
        camera_dir.z = cos(glm::radians(camera_angle));
        camera_dir = glm::normalize(camera_dir);
    };
};
#endif
