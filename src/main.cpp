#include "includes.h"

using namespace std;
namespace plt = ImGui;

mat4 controlsFunction(GLFWwindow* window);

int main() {
    general Data;

    int numberOfObjects;
    unsigned int PBO[2] = { 0, 0 };

    float maxVel, maxRD, X, Y, Z;

    // Inputs for particle assessment
    cout << "number of objects:", cin >> numberOfObjects, cout << "\n";
    cout << "maximum velocity(m/s):", cin >> maxVel, cout << "\n";
    cout << "maximum radius(m):", cin >> maxRD, cout << "\n";
    cout << "MaxX:", cin >> X, cout << "\n";
    cout << "MaxY:", cin >> Y, cout << "\n";
    cout << "MaxZ:", cin >> Z, cout << "\n";

    // Function which updates the general info into the general class
    Data.data(numberOfObjects, maxVel, maxRD, X, Y, Z);

    particle* particles = Data.particleArray();

    for (int i = 0; i < numberOfObjects; i++) {
        particles[i].dataCollector(i);
    }

    Data.gridAllocator();

    std::vector<float> velocities; // Stores the velocities to plot the graph 

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1200, 800, "CollisionSim", NULL, NULL);
    glfwMakeContextCurrent(window);
    functionLoader();
    windowStrecher(window);
    glViewport(0, 0, 1200, 800);

    ImGui::CreateContext();
    ImPlot::CreateContext();

    // Set default dark theme
    ImGui::StyleColorsDark();

    // Initialize GLFW and OpenGL3 backends for ImGui
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // All required vertices and indices
    float vertices[] = {
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    float box[] = {
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f
    };

    unsigned int EBO[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int boxEBO[] = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        1, 5, 2, 6, 3, 7, 0, 4
    };

    /*
     * Screen recording function
     * Takes care of creating and allocating respective buffers to capture events in collision simulator
     * For more definition look into recorder.cpp
     */
    screenRec(1200, 800, 60, "object_collision", PBO);

    /*
     * bufferIDs are returned as an array for binding
     * All the vertex binding and draw instance calls are contained in these functions
     */
    std::vector<unsigned int> bufferIDs = bufferID(
        vertices, sizeof(vertices),
        box, sizeof(box),
        EBO, sizeof(EBO),
        boxEBO, sizeof(boxEBO),
        numberOfObjects, particles
    );

    /*
     * configureOpenMPEnvironment() contains C++ commands to prevent core lags while switching between loops
     * Identifies the OS and manages commands to run without external dependencies
     */
    configureOpenMPEnvironment();

    /*
     * setDynCores() allows setting the percentage of available cores utilised for parallelism
     * Look at particleOperation file for more info
     */
    std::vector<int> coreInfo = setDynCores(0.90f);

    // Contains all the shader file compilation  
    unsigned int shaderProgram = shaderProgramLoader();

    // Projection matrix 
    mat4 proj = perspective(radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);
    mat4 view;

    // variableLocation() provides a vector consisting of Location IDs of respective variables inside shaders 
    std::vector<unsigned int> locations = variableLocation(shaderProgram);

    glUseProgram(shaderProgram);
    isRecord(1);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        /*
         * updateParticleInfo() executes the parallel loops and painter function
         * All parallelism is handled inside this function (visit particleOperation.cpp to know more)
         */
        updateParticleInfo(particles, numberOfObjects, velocities);

        // Frame initialisation and UI part
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        plt::NewFrame();
        plt::Begin("Maxwell_Boltzman_distribution");

        if (ImPlot::BeginPlot("Maxwell_boltzmann_distribution", ImVec2(-1.0f, 300.0f))) {
            ImGui::Text("frame rate : %.2f", ImGui::GetIO().Framerate);
            ImGui::Text("total cores available : %d", coreInfo[0]);
            ImGui::Text("total cores utilized : %d", coreInfo[1]);

            ImPlot::SetupAxes("velocity", "Fraction_of_particles", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxisLimits(ImAxis_X1, 0.0f, 0.5f, ImGuiCond_Once);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, numberOfObjects, ImGuiCond_Once);

            ImPlot::PlotHistogram("Particles", velocities.data(), numberOfObjects, 60);
            ImPlot::EndPlot();
        }

        plt::End();

        // updateInfo() function executes instance rendering and all major draw calls 
        updateInfo(window, locations, bufferIDs, numberOfObjects, particles, proj);

        plt::Render();
        ImGui_ImplOpenGL3_RenderDrawData(plt::GetDrawData());

        bufferReader(PBO);
        ifRecord(1, PBO);

        // Grid cleaning and buffer swapping
        Data.cleanGrid();
        screen(window);
        velocities.clear();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    delete[] particles;
    Data.deleteGrid();
    isRecord(0);
    glfwTerminate();

    return 0;
}