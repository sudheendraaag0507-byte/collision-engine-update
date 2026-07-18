#include "include/boilerPlate.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/*
	This struct stores the data about the bound VAO VBO EBO,
	holds the pointer to its ID , and stores the ID of the shaders in an static array.
	The data about the objects are stored in a dynamic array coz we may need n number of respective objects and
	this changes with each and every program.

*/
struct globalContextCurrentData
{
	unsigned int  number_of_VAO = 0;
	unsigned int  number_of_VBO = 0;
	unsigned int  number_of_EBO = 0;

	unsigned int* attributeObject = nullptr;
	unsigned int* bufferObject = nullptr;
	unsigned int* elementObject = nullptr;

	unsigned int  shaders[4] = { 0 };
};

globalContextCurrentData dataSet;

/*
	This struct carries the data about the texture.
	Here the data of all the variables are just initialised to the respective values,
	to prevent garbage values being allocated to the respective function when the argument is not passed.
*/

struct texture_data
{
	unsigned int dataColorFormat = GL_RGBA;
	unsigned int dataTexType = GL_UNSIGNED_BYTE;
	unsigned int textureDimension = GL_TEXTURE_2D;
	unsigned int wrap_s = GL_REPEAT;
	unsigned int wrap_t = GL_REPEAT;
	unsigned int wrap_r = GL_REPEAT;
	unsigned int magFilter = GL_LINEAR;
	unsigned int minFilter = GL_LINEAR;
};

texture_data texData;

/*
	The global variables used for tracing the mouse movements (mouseCallbacks) and scroll Callback.
*/

vec3 cameraFront = vec3(0.0f, 0.0f, 0.0f);
vec3 direction;
bool firstCoord = true;
float Zoom;
double lastX = 0.0f;
double lastY = 0.0f;

// Global variables for change in time

float lastFrame = 0.0f;
float curFrame = 0.0f;

string readShaderFile(const char* filePath)
{	
	// this function reads the code written in shader txt file and return the code as a string.  
	string   shaderCode;
	ifstream shaderFile;

	shaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		shaderFile.open(filePath);
		stringstream shaderStream;

		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		shaderCode = shaderStream.str();
	}
	catch (ifstream::failure& e)
	{
		cout << "ERROR::BOILERPLATE::FILE_NOT_SUCCESSFULLY_READ: " << filePath << endl;
	}

	return shaderCode;
}

unsigned int glCompileShaders(int n, const char** code)
{	
	//This function compiles the code(string) from the txt file .
	int          success;
	char         infoLog[512];
	unsigned int program = glCreateProgram();

	//the for loop is used here in order to keep this function flexible and improve it in future if there exist more than 2 shader.
	for (int i = 0; i < n; i++)
	{
		switch (i)
		{
		case (0):
		{
			unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
			dataSet.shaders[i] = vertexShader;
			glShaderSource(vertexShader, 1, &code[i], NULL);
			glCompileShader(vertexShader);
			glAttachShader(program, vertexShader);
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::cout << "ERROR::VERTEXSHADER::VERTEX::COMPILATION_FAILED\n"
					<< infoLog << std::endl;
				return 0;
			}
			break;
		}

		case (1):
		{
			unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			dataSet.shaders[i] = fragmentShader;
			glShaderSource(fragmentShader, 1, &code[i], NULL);
			glCompileShader(fragmentShader);
			glAttachShader(program, fragmentShader);
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::cout << "ERROR::FRAGMENTSHADER::VERTEX::COMPILATION_FAILED\n"
					<< infoLog << std::endl;
				return 0;
			}
			break;
		}
		}
	}
	glLinkProgram(program);
	return program;
}

//deletes the shader program in order to prevent memory leak.
void deletePrograms(int n)
{
	for (int i = 0; i < n; i++)
	{
		glDeleteShader(dataSet.shaders[i]);
	}
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void windowStrecher(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, windowStretch);
}

void windowStretch(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void functionLoader()
{
	if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
	{
		std::cout << "cant load functions!!!";
		glfwTerminate();
	}
}

void screen(GLFWwindow* window)
{
	glfwSwapBuffers(window);
	processInput(window);
	glfwPollEvents();
}

/*
	bufferCreater gets in the number of buffer objects, and create same number of EBO and VAO.
	Each VAO is capable of holding 16 different info for a given object and a dedicated EBO at last.
*/
void bufferCreater(unsigned int nVBO)
{
	unsigned int* VBO, * VAO, * EBO;
	VBO = new unsigned int[nVBO];
	VAO = new unsigned int[nVBO];
	EBO = new unsigned int[nVBO];

	glGenVertexArrays(nVBO, VAO);
	glGenBuffers(nVBO, VBO);

	dataSet.attributeObject = VAO;
	dataSet.bufferObject = VBO;
	dataSet.elementObject = EBO;
	dataSet.number_of_VAO = nVBO;
	dataSet.number_of_VBO = nVBO;
}

/*
	bufferAttacher binds the requested VAO and VBO , the respective EBO (if requested )
*/

void bufferAttacher(unsigned int indexVBO, unsigned indexVAO, unsigned int createEBO)
{
	unsigned int* VAO = dataSet.attributeObject;
	unsigned int* VBO = dataSet.bufferObject;

	glBindVertexArray(VAO[indexVAO]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[indexVBO]);

	//this if statement checks whether a EBO is requested.
	if (createEBO == 1)
	{	
		//this checks whether this EBO is previously requested and binded , in this way we can edit the EBO and create it for a given combination of VAO and VBO.
		if (dataSet.elementObject[indexVAO] != 0)
		{	
			//clears the buffer if it was already binded  
			glDeleteBuffers(1, &dataSet.elementObject[indexVAO]);
			
		}
		//creates a brand new EBO (that fetches a new id to bind with the EBO)
		glGenBuffers(1, &dataSet.elementObject[indexVAO]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dataSet.elementObject[indexVAO]);
	}

	if (createEBO == 2) {
		//jus binds the old one
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dataSet.elementObject[indexVAO]);
	}
}

/*
	bufferData gives the instruction on how to write data into that buffer.
*/
void bufferData(unsigned int mode, float* vertices, unsigned int sizevertex, unsigned int* indices,unsigned int sizeindex)
{
	glBufferData(GL_ARRAY_BUFFER, sizevertex, vertices, mode);
	if (indices != nullptr)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeindex, indices, mode);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	dataSet.number_of_EBO++;
}

/*
	bufferBinder is a function dedicated to bind the VAO, during the compilation of shaders as it contains the necessary info like texture,
	vertices , and colors...
*/
unsigned int bufferBinder(unsigned int indexVao)
{
	glBindVertexArray(dataSet.attributeObject[indexVao]);
	return dataSet.attributeObject[indexVao];
}

// this prevents memory leak after binding the respective objects .
void deleteArrays()
{
	glDeleteBuffers(dataSet.number_of_VAO, dataSet.attributeObject);
	glDeleteBuffers(dataSet.number_of_VAO, dataSet.bufferObject);
	glDeleteBuffers(dataSet.number_of_VBO, dataSet.elementObject);
	delete[] dataSet.attributeObject;
	delete[] dataSet.bufferObject;
	delete[] dataSet.elementObject;
}

unsigned int textureGen(unsigned int dimension, unsigned int textureUnit)
{
	switch (dimension)
	{
	case (1):
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_1D, texture);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, texData.wrap_s);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, texData.minFilter);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, texData.magFilter);
		return texture;
	}
	case (2):
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texData.wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texData.wrap_t);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texData.minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texData.magFilter);
		return texture;
	}
	case (3):
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_3D, texture);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, texData.wrap_s);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, texData.wrap_t);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, texData.wrap_r);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, texData.minFilter);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, texData.magFilter);
		return texture;
	}
	}
}

void textureData(unsigned int dimension, unsigned int dataFormat, unsigned int dataType)
{
	texData.dataColorFormat = dataFormat;
	texData.dataTexType = dataType;
	texData.textureDimension = dimension;
}

void textureParameters(unsigned int s, unsigned int t, unsigned int r, unsigned int magFilter,
	unsigned int minFilter)
{
	texData.wrap_s = s;
	texData.wrap_t = t;
	texData.wrap_r = r;
	texData.magFilter = magFilter;
	texData.minFilter = minFilter;
}

void textureLoader(const char* name, unsigned int miplevel, unsigned int colorFormat,
	unsigned int boundText)
{
	stbi_set_flip_vertically_on_load(true);
	glBindTexture(texData.textureDimension, boundText);

	int            width, height, nrChannels;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	unsigned char* data = stbi_load(name, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (texData.textureDimension == GL_TEXTURE_2D)
		{
			glTexImage2D(texData.textureDimension, miplevel, colorFormat, width, height, 0,
				texData.dataColorFormat, texData.dataTexType, data);
		}
		//-------------- need to write for 1D and 2D images as well--------------------
		glGenerateMipmap(texData.textureDimension);
		stbi_image_free(data);
	}
	else
	{
		cout << "Failed to load texture" << std::endl;
	}
}

void mouseTrace(GLFWwindow* window, double xPos, double yPos)
{
	static double pitch;
	static double yaw;

	if (firstCoord) {
		lastX = xPos;
		lastY = yPos;
		pitch = 0.0f;
		yaw = 0.0f;
		firstCoord = false;
	}

	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	float sensitivity = 0.2f;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	pitch += yoffset;
	yaw += xoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	if (yaw > 89.0f) {
		yaw = 89.0f;
	}
	else if (yaw < -89.0f) {
		yaw = -89.0f;
	}

	direction.x = cos(radians(pitch)) * sin(radians(yaw));
	direction.y = sin(radians(pitch));
	direction.z = sin(radians(pitch)) * cos(radians(yaw));
	cameraFront = (direction);


}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

void mouseInput(GLFWwindow* window) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseTrace);
	glfwSetScrollCallback(window, scroll_callback);

}

void mouseOffset(vec3* coordinates, float* fov)
{
	coordinates->x = cameraFront.x;
	coordinates->y = cameraFront.y;
	coordinates->z = cameraFront.z;
	if (!(fov == nullptr)) {
		*fov = Zoom;
	}
}

float delTime() {
	lastFrame = curFrame ;
	curFrame = glfwGetTime();

	return curFrame - lastFrame ;
}
