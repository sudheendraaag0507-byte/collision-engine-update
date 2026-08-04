#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

glm::mat4 controlsFunction(GLFWwindow* window) {
    static float angle = 0.0f;
    static float angle1 = 0.0f;
    static float scl = 1.0f;

    static glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    static glm::mat4 view2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    static glm::mat4 scale = glm::mat4(1.0f);
    static glm::mat4 viewTilt = glm::mat4(1.0f);

    viewTilt = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(angle1), glm::vec3(1.0f, 0.0f, 0.0f));

    scale = glm::scale(glm::mat4(1.0f), glm::vec3(scl));

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        view = view2 * viewTilt * scale;
        angle += 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        view = view2 * viewTilt * scale;
        angle -= 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        view = view2 * viewTilt * scale;
        angle1 -= 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        view = view2 * viewTilt * scale;
        angle1 += 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        view = view2 * viewTilt * scale;
        scl += 0.01f;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        view = view2 * viewTilt * scale;
        scl -= 0.01f;
    }

    return view;
}