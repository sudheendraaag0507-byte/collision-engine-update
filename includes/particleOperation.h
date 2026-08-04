#pragma once
#include "boilerPlate.h"
#include "particle.h"

class particle;
void updateParticleInfo(particle* particles, int numberOfObjects, std::vector<float>& velocities);
std::vector<int> setDynCores(float totalProcesserPercent);
void configureOpenMPEnvironment();
