#include <iostream>
#include <climits>
#include "include/boilerPlate.h"
#include "general.h"
#include "particle.h"
#include "grid.h"



    int general::numberOfParticle = 0;
    float general::maxVelocity = 0.0f;
    float general::maxRadius = 0.0f;
    float general::maxX = 0.0f;
    float general::maxY = 0.0f;
    float general::maxZ = 0.0f;
    int general::maxDivison = 0;
    float general::smallest_radii = INT_MAX;


    // here is where i tore all the info related to the simulation grid

    
    // this the radii math which checks whether the particles are actually hitting even though when the cells overlap

    bool general::radiiMath(vec3 position1, vec3 position2, float radius1, float radius2) {
        float sqdis = (position1.x - position2.x) * (position1.x - position2.x) + (position1.y - position2.y) * (position1.y - position2.y) + (position1.z - position2.z) * (position1.z - position2.z);

        if (sqdis <= (radius1 + radius2) * (radius1 + radius2)) {
            vec3 Prel = position1 - position2;

            //cout << Prel.x << " " << Prel.y << " " << length(Prel) << "<-----this is while hitting \n"; // this for debugging in case of any failure in future
            return true;
        }
        else {
            //vec3 Prel = position1 - position2;

            //cout << Prel.x << " " << Prel.y << " " << "<-----this is before or after hitting \n";

            return false;
        }
    }

    // this is the actual 2D collision math

    //conservation of linear moment is being used and here the velocities are passed by reference
    void general::collisionMath(float ds, float m1, float m2, float* v1, float* v2, vec3 position1, vec3 position2) {
        if (ds <= 0) {

            //first need to find the line of approach

                vec3 loa = normalize(position1 - position2) ;

            //now getting the magnitude of velocity of each particle

                float mag1 = length(vec3(v1[0],v1[1],v1[2]));

                float mag2 = length(vec3(v2[0], v2[1], v2[2]));

            //velocities reconstructed
                vec3 velocity1 = vec3(0.0f);
                vec3 velocity2 = vec3(0.0f);

            if(mag1 >= 0.0000001)
                 velocity1 = vec3(v1[0], v1[1], v1[2])/mag1;

            if (mag2 >= 0.0000001)
                 velocity2 = vec3(v2[0], v2[1], v2[2])/mag2;


            //length along loa 
            float vel1cos = 0;
            float vel2cos = 0;

            if (mag1 >= 0.0000001)
                vel1cos = dot(velocity1, loa) * mag1 ;

            if (mag2 >= 0.0000001)
                vel2cos = dot(velocity2, loa) * mag2 ;

            //velocities along loa
                
                vec3 loaVel1 = vel1cos * loa ;

                vec3 loaVel2 = vel2cos * loa ;

            //velocities that are perpendicular 

                vec3 perVel1 = mag1 * velocity1 - loaVel1 ;

                vec3 perVel2 = mag2 * velocity2 - loaVel2 ;

            //collision math , jus magnitudes

                float finalVelocity1 = ((m1 - m2) * vel1cos + 2 * m2 * vel2cos) / (m1 + m2) ;

                float finalVelocity2 = ( 2 * m1 * vel1cos + (m2 - m1) * vel2cos ) / (m1 + m2);

            //now multiplying it with the unit vector along the line and adding it perpendicular
                
                vec3 finalVel1 = finalVelocity1 * loa + perVel1 ;

                vec3 finalVel2 = finalVelocity2 * loa + perVel2 ;

                v1[0] = finalVel1.x ;
                v1[1] = finalVel1.y ;
                v1[2] = finalVel1.z ;

                v2[0] = finalVel2.x;
                v2[1] = finalVel2.y;
                v2[2] = finalVel2.z;
                
        }
    }

    // this function creates a virtual grid in the RAM 
    //void gridAllocator();

    // this function wipes clean all the data registered in each cell
    //void cleanGrid();

    // as we have dynamically allocated memory here , so we need to delete the virtual grid in order prevent memory leak using this function
    //void deleteGrid();

particle* general::particleArray() {
    return particles;
}

void general::gridAllocator() {
    maxDivison = int(8 * (1 / smallest_radii));
    
    cell = new grid[8 * maxDivison * maxDivison * maxDivison]; // using 1D array to avoid cache miss
    
}

void general::cleanGrid() {
    for (int i = 0; i < 8 * maxDivison * maxDivison * maxDivison; i++) {
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