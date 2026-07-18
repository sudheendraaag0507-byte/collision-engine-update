#include <iostream>
#include "include/boilerPlate.h"
#include "general.h"
#include "particle.h"
#include "grid.h"


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

        divisonX = (maxDivison)*shiftedPositionX;

        divisonY = (maxDivison)*shiftedPositionY;// 8 is used in order to decrease the grid size

        divisonZ = (maxDivison)*shiftedPositionY;
    }

    // this function fills the individual cells present inside the grid based on the balls position
    //void particle::painter();

    void particle::dataCollector(int i) {
        std::cout << "mass of object (KG) " << i + 1 << ":";
        std::cin >> particles[i].mass, std::cout << "\n";
        std::cout << "velocity of object " << i + 1 << ":\n";
        std::cout << "X:", std::cin >> particles[i].velocity.x, std::cout << "\n";
        std::cout << "Y:", std::cin >> particles[i].velocity.y, std::cout << "\n";
        std::cout << "Z:", std::cin >> particles[i].velocity.z, std::cout << "\n";
        std::cout << "position of object " << i + 1 << ":\n";
        std::cout << "X:", std::cin >> particles[i].position.x, std::cout << "\n";
        std::cout << "Y:", std::cin >> particles[i].position.y, std::cout << "\n";
        std::cout << "Z:", std::cin >> particles[i].position.z, std::cout << "\n";
        std::cout << "radius of object " << i + 1 << ":\n";
        std::cin >> particles[i].radius;

        particles[i].velocity.x = particles[i].velocity.x / maxVelocity;
        particles[i].velocity.y = particles[i].velocity.y / maxVelocity;
        particles[i].velocity.z = particles[i].velocity.z / maxVelocity;

        particles[i].position.x = particles[i].position.x / maxX;
        particles[i].position.y = particles[i].position.y / maxY;
        particles[i].position.z = particles[i].position.z / maxZ;

        particles[i].radius = particles[i].radius / maxRadius;

        if (smallest_radii > particles[i].radius) {
            smallest_radii = particles[i].radius;
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
        int scale = (radius * maxDivison);

        //change this , carefull!!!
        
        for (int i = std::max(divisonX - scale, 0); i <= std::min(divisonX + scale, 2 * maxDivison - 1); i++) {
            for (int j = std::max(divisonY - scale, 0); j <= std::min(divisonY + scale, 2 * maxDivison - 1); j++) {
                for (int k = std::max(divisonZ - scale, 0); k <= std::min(divisonZ + scale, 2 * maxDivison - 1); k++){
                    (cell + i + 2 * j * maxDivison + 4 * maxDivison * maxDivison * k )->fill(ballNumber);
                }
            }
        }
        cell->cleanlist();
    }

particle* general::particles = nullptr;
