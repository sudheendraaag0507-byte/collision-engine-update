#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define _1D 1
#define _2D 2
#define _3D 3

using namespace std;
using namespace glm;

unsigned int glCompileShaders(int n, const char** code);
string       readShaderFile(const char* filePath);
void         deletePrograms(int n);
void         deleteArrays();
void         processInput(GLFWwindow* window);
void         windowStrecher(GLFWwindow* window);
void         windowStretch(GLFWwindow* window, int width, int height);
void         functionLoader();
void         screen(GLFWwindow* window);
void         bufferAttacher(unsigned int indexVBO, unsigned indexVAO, unsigned int createEBO);
void         bufferCreater(unsigned int nVBO);
void         bufferData(unsigned int mode, float* vertices, unsigned int sizevertex,
	unsigned int* indices, unsigned int sizeindex);
unsigned int bufferBinder(unsigned int indexVao);
void         textureData(unsigned int dimension, unsigned int dataFormat, unsigned int dataType);
void         textureLoader(const char* name, unsigned int miplevel, unsigned int colorFormat,
	unsigned int boundText);
unsigned int textureGen(unsigned int dimension, unsigned int textureUnit);
void         textureParameters(unsigned int s, unsigned int t, unsigned int r,
	unsigned int magFilter, unsigned int minFilter);
void		 mouseTrace(GLFWwindow* window, float xPos, float yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouseInput(GLFWwindow* window);
void mouseOffset(vec3* coordinates, float* fov);
float delTime();
