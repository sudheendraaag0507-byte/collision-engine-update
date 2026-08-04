#pragma once

#include "boilerPlate.h"
#include "particle.h"


void updateInfo(GLFWwindow* window, std::vector<unsigned int> arr, std::vector<unsigned int> buffers , int numberOfObjects, particle* particles, mat4 proj);
void drawObject();
std::vector<unsigned int> variableLocation(unsigned int shaderProgram);