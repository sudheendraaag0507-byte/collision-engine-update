#include<iostream>
#include "include/boilerPlate.h"
#include "general.h"
#include "particle.h"
#include "grid.h"

    // here is where i tore all the info related to the simulation grid

    
    // this the radii math which checks whether the particles are actually hitting even though when the cells overlap

    bool general::radiiMath(vec3 position1, vec3 position2, float radius1, float radius2) {
        float sqdis = (position1.x - position2.x) * (position1.x - position2.x) + (position1.y - position2.y) * (position1.y - position2.y);

        if (sqdis <= (radius1 + radius2) * (radius1 + radius2)) {
            vec3 Prel = position1 - position2;

            //cout << Prel.x << " " << Prel.y << " " << length(Prel) << "<-----this is while hitting \n"; // this for debugging in case of any failure in future
            return true;
        }
        else {
            vec3 Prel = position1 - position2;

            //cout << Prel.x << " " << Prel.y << " " << "<-----this is before or after hitting \n";

            return false;
        }
    }

    // this is the actual 2D collision math

    //conservation of linear moment is being used and here the velocities are passed by reference
    void general::collisionMath(float ds, float m1, float m2, float* v1, float* v2, vec3 position1, vec3 position2) {
        if (ds <= 0) {

            float theta = atan2((position1.y - position2.y), (position1.x - position2.x));
            float cos = cosf(theta);
            float sin = sinf(theta);

            // the math is being done along the line of approach

            float old_v1x = v1[0] * cos + v1[1] * sin;
            float old_v2x = v2[0] * cos + v2[1] * sin;
            float old_v1y = v1[1] * cos - v1[0] * sin;
            float old_v2y = v2[1] * cos - v2[0] * sin;

            float new_v1x = ((m1 - m2) * old_v1x + 2 * m2 * old_v2x) / (m1 + m2);
            float new_v2x = (2 * m1 * old_v1x + (m2 - m1) * old_v2x) / (m1 + m2);
            float new_v1y = old_v1y;
            float new_v2y = old_v2y;

            v1[0] = new_v1x * cos - new_v1y * sin;
            v2[0] = new_v2x * cos - new_v2y * sin;
            v1[1] = new_v1x * sin + new_v1y * cos;
            v2[1] = new_v2x * sin + new_v2y * cos;
        }
    }

    // this function creates a virtual grid in the RAM 
    //void gridAllocator();

    // this function wipes clean all the data registered in each cell
    //void cleanGrid();

    // as we have dynamically allocated memory here , so we need to delete the virtual grid in order prevent memory leak using this function
    //void deleteGrid();


int general::numberOfParticle = 0;
float general::maxVelocity = 0.0f;
float general::maxRadius = 0.0f;
float general::maxX = 0.0f;
float general::maxY = 0.0f;
float general::maxZ = 0.0f;
int general::maxDivison = 0;
float general::smallest_radii = INT_MAX;

particle* general::particleArray() {
    return particles;
}

void general::gridAllocator() {
    maxDivison = 8 * (1 / smallest_radii);
    cell = new grid[4 * maxDivison * maxDivison];
}

void general::cleanGrid() {
    for (int i = 0; i < 4 * maxDivison * maxDivison; i++) {
        cell[i].clean();
    }
}

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