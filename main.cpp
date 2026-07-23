#include "include\boilerPlate.h"
#include "include\Recorder.h"
#include "collisionSim2D-3D/general.h"
#include "collisionSim2D-3D/grid.h"
#include "collisionSim2D-3D/particle.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

using namespace std;

namespace plt = ImGui ;

mat4 controlsFunction(GLFWwindow* window) ;

//early initialisation of static variable

int main() {
    general Data;
    
	int numberOfObjects;
    unsigned int PBO[2] = {0,0};
    
    float maxVel , maxRD , X , Y , Z;

    cout << "number of objects:", cin >> numberOfObjects, cout << "\n";
    cout << "maximum velocity(m/s):", cin >> maxVel, cout << "\n";
    cout << "maximum radius(m):", cin >> maxRD, cout << "\n";
    cout << "MaxX:", cin >> X, cout << "\n";
    cout << "MaxY:", cin >> Y, cout << "\n";
    cout << "MaxZ:", cin >> Z, cout << "\n";

	Data.data(numberOfObjects,maxVel,maxRD,X,Y,Z);

    particle* particles = Data.particleArray();

    for (int i = 0; i < numberOfObjects; i++) {
        particles[i].dataCollector(i);
    }
    
    Data.gridAllocator();
    std::vector<float> velocities ;
    
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(1200,800,"CollisionSim", NULL, NULL);
	glfwMakeContextCurrent(window);
	functionLoader();
	windowStrecher(window);
	glViewport(0,0,1200,800);

    ImGui::CreateContext();
    ImPlot::CreateContext();

    // Set default dark theme
    ImGui::StyleColorsDark();

    // Initialize GLFW and OpenGL3 backends for ImGui
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

	float vertices[] = {
		0.5f,0.5f,0.0f,
		-0.5f,0.5f,0.0f,
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f
	};

    float box[] = {
        -1.0f,1.0f,1.0f,
        -1.0f,1.0,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,1.0f,
         1.0f,1.0f,1.0f,
         1.0f,1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f,-1.0f,1.0f,
    };

	unsigned int EBO[] = {
		0,1,2,
		2,3,0
	};

    unsigned int boxEBO[] = {
        0,1 ,1,2 ,2,3 ,3,0,
        4,5 ,5,6 ,6,7 ,7,4,
        1,5 ,2,6 ,3,7 ,0,4
    };

    screenRec(1200, 800, 60, "object_collision", PBO);

	bufferCreater(2);
	bufferAttacher(0,0,1);
	bufferData(GL_STATIC_DRAW,vertices,sizeof(vertices),EBO,sizeof(EBO));
	unsigned int VAO = bufferBinder(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

    //i am trying to create a buffer to hold in the data regarding the particle for instance drawing 

    unsigned int instance;
    glGenBuffers(1, &instance);
    glBindBuffer(GL_ARRAY_BUFFER, instance);
    glBufferData(GL_ARRAY_BUFFER, numberOfObjects * sizeof(particle), particles, GL_STATIC_DRAW);

    glVertexAttribPointer(2 , 3 , GL_FLOAT , GL_FALSE , sizeof(particle) , (void*)8 );
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(particle), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3,1);

    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(particle), (void*)20);
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);

    bufferAttacher(1, 1, 1);
    bufferData(GL_STATIC_DRAW, box, sizeof(box), boxEBO, sizeof(boxEBO));
    unsigned int boxVAO = bufferBinder(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


	string vertexShader = readShaderFile("vshader.txt");
	string fragmentShader = readShaderFile("fshader.txt");
	const char* vcode = vertexShader.c_str();
	const char* fcode = fragmentShader.c_str();
	const char* shaderCode[] = {vcode , fcode};
	unsigned int shaderProgram = glCompileShaders(2,shaderCode);
	

    //projection matrix 

    mat4 proj = perspective(radians(45.0f),1200.0f/800.0f,0.1f,100.0f);
    mat4 view ;
    unsigned int loc1 = glGetUniformLocation(shaderProgram, "view");
    unsigned int loc0 = glGetUniformLocation(shaderProgram , "projection");
    unsigned int loc2 = glGetUniformLocation(shaderProgram, "is_box");
    unsigned int loc3 = glGetUniformLocation(shaderProgram, "lightPos");
    glUseProgram(shaderProgram);
    
    isRecord(1);
    
    glEnable(GL_DEPTH_TEST);

    
	while (!glfwWindowShouldClose(window)) {
        
		glClearColor(0.0,0.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //frame initialisation
        ImGui_ImplOpenGL3_NewFrame();

        ImGui_ImplGlfw_NewFrame();
        
        plt::NewFrame();

        plt::Begin("Maxwell_Boltzman_distribution");

        
        
        
		for (int n = 0; n < numberOfObjects; n++) {

            particles[n].divisionCalculator();// calculate the division at which the center of the ball is present

            particles[n].painter();//paint that entire spanning square
            
            particles[n].changePosition();// changes the position

            particles[n].changeVelocity();// changes the velocity if crossin the border

            velocities.push_back(0.5* particles[n].m()*dot(particles[n].vel(),particles[n].vel()));
		}

        if (ImPlot::BeginPlot("Maxwell_boltzmann_distribution", ImVec2(-1.0f, 300.0f))) {
            ImPlot::SetupAxes("Kinetic_Energy", "Fraction_of_particles", ImPlotAxisFlags_None , ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxisLimits(ImAxis_X1, 0.0f,0.2f, ImGuiCond_Once);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, numberOfObjects, ImGuiCond_Once);
            
            ImPlot::PlotHistogram("Particles",velocities.data(), numberOfObjects, 100 );
            ImPlot::EndPlot();
        };

        

        plt::End();
        glUniform3fv(loc3 , 1 , value_ptr(vec3(0.0f,0.0f,0.0f)));
        
        glBindVertexArray(VAO);
        view = controlsFunction(window);
        
        glBindBuffer(GL_ARRAY_BUFFER, instance);
        glBufferData(GL_ARRAY_BUFFER , numberOfObjects * sizeof(particle), particles, GL_STATIC_DRAW);
        glUniformMatrix4fv(loc0 , 1 , GL_FALSE , value_ptr(proj));
        glUniformMatrix4fv(loc1, 1, GL_FALSE, value_ptr(view));
        glUniform1f(loc2 , 0);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 , numberOfObjects);

        
        glBindVertexArray(boxVAO);
        glUniformMatrix4fv(loc1, 1, GL_FALSE, value_ptr(view));
        glUniform1f(loc2 , 1);
        glDrawElements(GL_LINES , 24 , GL_UNSIGNED_INT , nullptr);

        bufferReader(PBO);
        ifRecord(1, PBO);
        
        Data.cleanGrid();

        plt::Render();
        ImGui_ImplOpenGL3_RenderDrawData(plt::GetDrawData());

		screen(window);
        velocities.clear();
		
	}
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    delete[]particles ;
    Data.deleteGrid();
    isRecord(0);
	glfwTerminate();
	return 0;
}

