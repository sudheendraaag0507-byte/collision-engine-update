#include <iostream>
#include "boilerPlate.h"
#include "general.h"
#include "particle.h"
#include "grid.h"
#include "omp.h"

// Static member definitions
float particle::maxM = 0.0f;
particle* general::particles = nullptr;

float particle::maxm() {
    return maxM;
}

vec3& particle::vel() {
    return velocity;
}

float particle::m() {
    return mass;
}

float particle::rad() {
    return radius;
}

vec3& particle::pos() {
    return position;
}

// Calculates the cell in which the particle's centre belongs to
void particle::divisionCalculator() {
    float shiftedPositionX = position.x + 1.0f;
    float shiftedPositionY = position.y + 1.0f;
    float shiftedPositionZ = position.z + 1.0f;

    divisonX = general::maxDivison * shiftedPositionX;
    divisonY = general::maxDivison * shiftedPositionY;
    divisonZ = general::maxDivison * shiftedPositionZ;
}

void particle::dataCollector(int i) {
    std::cout << "mass of object (KG) " << i + 1 << ":";
    std::cin >> general::particles[i].mass;
    std::cout << "\n";

    std::cout << "velocity of object " << i + 1 << ":\n";
    std::cout << "X:";
    std::cin >> general::particles[i].velocity.x;
    std::cout << "\n";
    std::cout << "Y:";
    std::cin >> general::particles[i].velocity.y;
    std::cout << "\n";
    std::cout << "Z:";
    std::cin >> general::particles[i].velocity.z;
    std::cout << "\n";

    std::cout << "position of object " << i + 1 << ":\n";
    std::cout << "X:";
    std::cin >> general::particles[i].position.x;
    std::cout << "\n";
    std::cout << "Y:";
    std::cin >> general::particles[i].position.y;
    std::cout << "\n";
    std::cout << "Z:";
    std::cin >> general::particles[i].position.z;
    std::cout << "\n";

    std::cout << "radius of object " << i + 1 << ":\n";
    std::cin >> general::particles[i].radius;

    // Normalizing attributes relative to bounding box and maximum thresholds
    general::particles[i].velocity.x /= general::maxVelocity;
    general::particles[i].velocity.y /= general::maxVelocity;
    general::particles[i].velocity.z /= general::maxVelocity;

    general::particles[i].position.x /= general::maxX;
    general::particles[i].position.y /= general::maxY;
    general::particles[i].position.z /= general::maxZ;

    general::particles[i].radius /= general::maxRadius;

    if (general::smallest_radii > general::particles[i].radius) {
        general::smallest_radii = general::particles[i].radius;
    }

    if (maxM < general::particles[i].mass) {
        maxM = general::particles[i].mass;
    }

    ballNumber = i;
}

/*
 * Defines containment boundaries
 * Dot product prevents particle locking at containment edges
 */
void particle::changeVelocity() {
    if ((position.x >= 1.0f - radius || position.x <= -1.0f + radius) && (dot(velocity.x, position.x) > 0)) {
        velocity.x *= -1.0f;
    }
    if ((position.y >= 1.0f - radius || position.y <= -1.0f + radius) && (dot(velocity.y, position.y) > 0)) {
        velocity.y *= -1.0f;
    }
    if ((position.z >= 1.0f - radius || position.z <= -1.0f + radius) && (dot(velocity.z, position.z) > 0)) {
        velocity.z *= -1.0f;
    }
}

void particle::changePosition() {
    position.x += velocity.x * 0.1f;
    position.y += velocity.y * 0.1f;
    position.z += velocity.z * 0.1f;
}

void particle::painter() {
    // Calculates total voxels spanned by the particle
    int scale = static_cast<int>(radius * general::maxDivison);

    // Limits for iteration to enable parallel execution in OpenMP loops
    int i_start = std::max(divisonX - scale, 0);
    int i_end = std::min(divisonX + scale, static_cast<int>(2 * general::maxDivison - 1));

    int j_start = std::max(divisonY - scale, 0);
    int j_end = std::min(divisonY + scale, static_cast<int>(2 * general::maxDivison - 1));

    int k_start = std::max(divisonZ - scale, 0);
    int k_end = std::min(divisonZ + scale, static_cast<int>(2 * general::maxDivison - 1));

//#pragma omp parallel for collapse(3)
    for (int i = i_start; i <= i_end; i++) {
        for (int j = j_start; j <= j_end; j++) {
            for (int k = k_start; k <= k_end; k++) {
                if (scale * scale >= (i - divisonX) * (i - divisonX) + (j - divisonY) * (j - divisonY) + (k - divisonZ) * (k - divisonZ)) {
                    (general::cell + i + 2 * j * general::maxDivison + 4 * general::maxDivison * general::maxDivison * k)->fill(ballNumber);
                }
            }
        }
    }
}