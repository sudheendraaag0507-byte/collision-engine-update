#include "objectDrawing.h"
#include "boilerPlate.h"
#include "particle.h"

mat4 controlsFunction(GLFWwindow* window);

std::vector<unsigned int> variableLocation(unsigned int shaderProgram) {
    unsigned int loc0 = glGetUniformLocation(shaderProgram, "projection");
    unsigned int loc1 = glGetUniformLocation(shaderProgram, "view");
    unsigned int loc2 = glGetUniformLocation(shaderProgram, "is_box");
    unsigned int loc3 = glGetUniformLocation(shaderProgram, "lightPos");
    unsigned int loc4 = glGetUniformLocation(shaderProgram, "scale");

    std::vector<unsigned int> arr = { loc0, loc1, loc2, loc3 , loc4 };

    return arr;
}

void updateInfo(
    GLFWwindow* window,
    std::vector<unsigned int> arr,
    std::vector<unsigned int> buffers,
    int numberOfObjects,
    particle* particles,
    mat4 proj
) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform3fv(arr[3], 1, value_ptr(vec3(0.0f, 0.0f, 0.0f)));

    glBindVertexArray(buffers[0]);
    std::vector<mat4> tempMatrix = controlsFunction(window);
    mat4 view = tempMatrix[0];
    mat4 particle_scale = tempMatrix[1];

    // Updates particle data in the buffer for instanced rendering
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, numberOfObjects * sizeof(particle), particles, GL_STATIC_DRAW);

    // Instanced draw call for particles
    glUniformMatrix4fv(arr[0], 1, GL_FALSE, value_ptr(proj));
    glUniformMatrix4fv(arr[1], 1, GL_FALSE, value_ptr(view));
    glUniform1f(arr[2], 0.0f);
    glUniformMatrix4fv(arr[3] , 1 , GL_FALSE , value_ptr(particle_scale));
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, numberOfObjects);

    // Draw call for the bounding box
    glBindVertexArray(buffers[2]);
    glUniformMatrix4fv(arr[1], 1, GL_FALSE, value_ptr(view));
    glUniform1f(arr[2], 1.0f);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, nullptr);
}
