#pragma once
#include "boilerPlate.h"
#include "particle.h"

std::vector <unsigned int> bufferID(float* vertices,int vertice , float* box, int boxSize , unsigned int* EBO, int EBOSize , unsigned int* boxEBO,int boxEBOSize , int numberOfObjects, particle* particles);
