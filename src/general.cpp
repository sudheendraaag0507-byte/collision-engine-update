#include <iostream>
#include <climits>
#include "boilerPlate.h"
#include "general.h"
#include "particle.h"
#include "grid.h"
#include "omp.h"

// Static member definitions for class general
int general::numberOfParticle = 0;
float general::maxVelocity = 0.0f;
float general::maxRadius = 0.0f;
float general::maxX = 0.0f;
float general::maxY = 0.0f;
float general::maxZ = 0.0f;
long long int general::maxDivison = 0;
float general::smallest_radii = INT_MAX;

// Checks whether particles are physically colliding even when bounding cells overlap
bool general::radiiMath(vec3 position1, vec3 position2, float radius1, float radius2) {
    float sqdis = (position1.x - position2.x) * (position1.x - position2.x) +
        (position1.y - position2.y) * (position1.y - position2.y) +
        (position1.z - position2.z) * (position1.z - position2.z);

    if (sqdis <= (radius1 + radius2) * (radius1 + radius2)) {
        vec3 Prel = position1 - position2;
        return true;
    }
    else {
        return false;
    }
}

// 3D elastic collision resolution using conservation of linear momentum
void general::collisionMath(float ds, float m1, float m2, float* v1, float* v2, vec3 position1, vec3 position2) {
    if (ds <= 0) {
        // Find line of approach (LOA)
        vec3 loa = normalize(position1 - position2);

        // Get magnitude of velocity for each particle
        float mag1 = length(vec3(v1[0], v1[1], v1[2]));
        float mag2 = length(vec3(v2[0], v2[1], v2[2]));

        // Normalize direction vectors
        vec3 velocity1 = vec3(0.0f);
        vec3 velocity2 = vec3(0.0f);

        if (mag1 >= 0.0000001f) {
            velocity1 = vec3(v1[0], v1[1], v1[2]) / mag1;
        }

        if (mag2 >= 0.0000001f) {
            velocity2 = vec3(v2[0], v2[1], v2[2]) / mag2;
        }

        // Projection length along LOA 
        float vel1cos = 0.0f;
        float vel2cos = 0.0f;

        if (mag1 >= 0.0000001f) {
            vel1cos = dot(velocity1, loa) * mag1;
        }

        if (mag2 >= 0.0000001f) {
            vel2cos = dot(velocity2, loa) * mag2;
        }

        // Velocity vectors along LOA
        vec3 loaVel1 = vel1cos * loa;
        vec3 loaVel2 = vel2cos * loa;

        // Perpendicular velocity components 
        vec3 perVel1 = mag1 * velocity1 - loaVel1;
        vec3 perVel2 = mag2 * velocity2 - loaVel2;

        // Elastic collision 1D momentum math along LOA
        float finalVelocity1 = ((m1 - m2) * vel1cos + 2.0f * m2 * vel2cos) / (m1 + m2);
        float finalVelocity2 = (2.0f * m1 * vel1cos + (m2 - m1) * vel2cos) / (m1 + m2);

        // Reconstruct final 3D velocity vectors
        vec3 finalVel1 = finalVelocity1 * loa + perVel1;
        vec3 finalVel2 = finalVelocity2 * loa + perVel2;

        v1[0] = finalVel1.x;
        v1[0] = finalVel1.x;
        v1[1] = finalVel1.y;
        v1[2] = finalVel1.z;

        v2[0] = finalVel2.x;
        v2[1] = finalVel2.y;
        v2[2] = finalVel2.z;
    }
}

particle* general::particleArray() {
    return particles;
}

// Allocates virtual spatial grid in memory based on smallest particle radius
void general::gridAllocator() {
    maxDivison = static_cast<long long int>(4.0f * (1.0f / smallest_radii));

    // 1D contiguous array allocation to maximize L1/L2 cache hits
    cell = new grid[8 * maxDivison * maxDivison * maxDivison];

    std::cout << "this is size of all the voxels combined: "
        << 8 * maxDivison * maxDivison * maxDivison * sizeof(cell[0])
        << "\n";
}

// Clears registered particle data in each spatial cell in parallel
void general::cleanGrid() {
#pragma omp parallel for 
    for (int i = 0; i < 8 * maxDivison * maxDivison * maxDivison; i++) {
        cell[i].clean();
    }
}

// Deallocates dynamic grid memory
void general::deleteGrid() {
    delete[] cell;
}

void general::data(int n, float Vel, float Radius, float X, float Y, float Z) {
    numberOfParticle = n;
    maxVelocity = Vel;
    maxRadius = Radius;
    maxX = X;
    maxY = Y;
    maxZ = Z;
    particles = new particle[n];
}