#include "buffers.h"
#include "boilerPlate.h"
#include "particle.h"

std::vector<unsigned int> bufferID(
    float* vertices,
    int vertice,
    float* box,
    int boxSize,
    unsigned int* EBO,
    int EBOSize,
    unsigned int* boxEBO,
    int boxEBOSize,
    int numberOfObjects,
    particle* particles
) {
    // creates the particles
    bufferCreater(2);
    bufferAttacher(0, 0, 1);
    bufferData(GL_STATIC_DRAW, vertices, vertice, EBO, EBOSize);

    unsigned int VAO = bufferBinder(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // Instanced Buffer Setup (particle data array)
    unsigned int instance;
    glGenBuffers(1, &instance);
    glBindBuffer(GL_ARRAY_BUFFER, instance);
    glBufferData(GL_ARRAY_BUFFER, numberOfObjects * sizeof(particle), particles, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (void*)8);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    // Radius attribute
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(particle), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    // Velocity attribute
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(particle), (void*)20);
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);

    // Bounding Box VAO Setup
    bufferAttacher(1, 1, 1);
    bufferData(GL_STATIC_DRAW, box, boxSize, boxEBO, boxEBOSize);

    unsigned int boxVAO = bufferBinder(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    std::vector<unsigned int> bufferIDs = { VAO, instance, boxVAO };

    return bufferIDs;
}