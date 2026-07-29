#include <iostream>
#include "include/boilerPlate.h"
#include "general.h"
#include "particle.h"
#include "grid.h"
#include "omp.h"

    float particle::maxM = 0.0f;
    
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

    // this function calculate the cell in which the particle's centre belong to 
    void particle::divisionCalculator() {
        // need to be careful that when the position vector is passed it is NDC the converted form is stored in the class!!!

        float shiftedPositionX = position.x + 1.0;
        float shiftedPositionY = position.y + 1.0;
        float shiftedPositionZ = position.z + 1.0;

        divisonX = (general::maxDivison)*shiftedPositionX;

        divisonY = (general::maxDivison)*shiftedPositionY;// 8 is used in order to decrease the grid size

        divisonZ = (general::maxDivison)*shiftedPositionZ;
    }

    // this function fills the individual cells present inside the grid based on the balls position
    //void particle::painter();

    void particle::dataCollector(int i) {
        std::cout << "mass of object (KG) " << i + 1 << ":";
        std::cin >> general::particles[i].mass, std::cout << "\n";
        std::cout << "velocity of object " << i + 1 << ":\n";
        std::cout << "X:", std::cin >> general::particles[i].velocity.x, std::cout << "\n";
        std::cout << "Y:", std::cin >> general::particles[i].velocity.y, std::cout << "\n";
        std::cout << "Z:", std::cin >> general::particles[i].velocity.z, std::cout << "\n";
        std::cout << "position of object " << i + 1 << ":\n";
        std::cout << "X:", std::cin >> general::particles[i].position.x, std::cout << "\n";
        std::cout << "Y:", std::cin >> general::particles[i].position.y, std::cout << "\n";
        std::cout << "Z:", std::cin >> general::particles[i].position.z, std::cout << "\n";
        std::cout << "radius of object " << i + 1 << ":\n";
        std::cin >> general::particles[i].radius;

        general::particles[i].velocity.x = general::particles[i].velocity.x / general::maxVelocity;
        general::particles[i].velocity.y = general::particles[i].velocity.y / general::maxVelocity;
        general::particles[i].velocity.z = general::particles[i].velocity.z / general::maxVelocity;

        general::particles[i].position.x = general::particles[i].position.x / general::maxX;
        general::particles[i].position.y = general::particles[i].position.y / general::maxY;
        general::particles[i].position.z = general::particles[i].position.z / general::maxZ;

        general::particles[i].radius = general::particles[i].radius / general::maxRadius;

        if (general::smallest_radii > general::particles[i].radius) {
            general::smallest_radii = general::particles[i].radius;
        }

        if (maxM < general::particles[i].mass) {
            maxM = general::particles[i].mass;
        }
    
        ballNumber = i;
    }

    // this function defines the containment 
    void particle::changeVelocity() {
        if ((position.x >= 1 - radius || position.x <= -1 + radius) && (dot(velocity.x, position.x) > 0)) {
            //dot product is being used in order to prevent the particle locking at the edge of the containment
            velocity.x *= (-1);
        }
        if ((position.y >= 1 - radius || position.y <= -1 + radius) && (dot(velocity.y, position.y) > 0)) {
            velocity.y *= (-1);
        }

        if ((position.z >= 1 - radius || position.z <= -1 + radius) && (dot(velocity.z, position.z) > 0)) {
            velocity.z *= (-1);
        }

    }

    void particle::changePosition() {
        position.x += velocity.x * (0.1);
        position.y += velocity.y * (0.1);
        position.z += velocity.z * (0.1);
    }

    void particle::painter() {
        int scale = (radius * general::maxDivison);

        int i_start = std::max(divisonX - scale, 0);
        int i_end = std::min(divisonX + scale, 2 * general::maxDivison - 1);

        int j_start = std::max(divisonY - scale, 0);
        int j_end = std::min(divisonY + scale, 2 * general::maxDivison - 1);

        int k_start = std::max(divisonZ - scale, 0);
        int k_end = std::min(divisonZ + scale, 2 * general::maxDivison - 1);

        
        
        for (int i = i_start; i <= i_end; i++) {
            for (int j = j_start; j <= j_end; j++) {
                for (int k = k_start; k <= k_end; k++) {
                    float rhs = (i - divisonX) * (i - divisonX) + (j - divisonY) * (j - divisonY) + (k - divisonZ) * (k - divisonZ) ;
                    float lhs = scale * scale ;
                    
                    if (lhs >= rhs ) {
                            (general::cell + i + 2 * j * general::maxDivison + 4 * general::maxDivison * general::maxDivison * k)->fill(ballNumber);
                    }
                }
            }
        }

        general::cell->cleanlist();
    }

particle* general::particles = nullptr;
