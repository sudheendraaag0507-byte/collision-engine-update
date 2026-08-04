#include "particleOperation.h"
#include "boilerPlate.h"
#include "particle.h"
#include <iostream>
#include <omp.h>
#include <cstdlib>

void configureOpenMPEnvironment() {
#ifdef _WIN32
    // 1. OMP_PROC_BIND=TRUE: Pins threads to specific physical CPU cores (prevents thread jumping)
    _putenv_s("OMP_PROC_BIND", "TRUE");

    // 2. OMP_WAIT_POLICY=ACTIVE: Keeps worker threads awake and spinning between loops (prevents thread sleep lag)
    _putenv_s("OMP_WAIT_POLICY", "ACTIVE");
#else
    // Linux / macOS equivalent
    setenv("OMP_PROC_BIND", "TRUE", 1);
    setenv("OMP_WAIT_POLICY", "ACTIVE", 1);
#endif
}

std::vector<int> setDynCores(float totalProcessorPercent = 0.70f) {
    int totalProcessorsAvailable = omp_get_num_procs();

    int processorUtilised = std::max(1, static_cast<int>(totalProcessorsAvailable * totalProcessorPercent));

    omp_set_dynamic(0);
    omp_set_num_threads(processorUtilised);

    std::vector<int> coreInfo = { totalProcessorsAvailable, processorUtilised };

    return coreInfo;
}

void updateParticleInfo(particle* particles, int numberOfObjects, std::vector<float>& velocities) {
    static int count = 0;


    #pragma omp parallel for schedule(guided,64)
        for (int n = 0; n < numberOfObjects; n++) {
            // Calculate the division at which the center of the ball is present
            particles[n].divisionCalculator();
        
            // Paint that entire spanning square
            particles[n].painter();
        
            // Change the position and check boundary conditions
            particles[n].changePosition();
            particles[n].changeVelocity();
        }
    

    if (count%100 == 0) {
        
        for (int n = 0; n < numberOfObjects; n++) {
            velocities.push_back(length(particles[n].vel()));
        }
    }

    count++;

    if(count > 100000) count = 0 ;
}