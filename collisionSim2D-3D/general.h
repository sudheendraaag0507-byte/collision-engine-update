#pragma once
#include <iostream>

class grid;
class particle;

// here is where i store all the info related to the simulation grid
class general {
protected:
    static int numberOfParticle;
    static float maxVelocity;
    static float maxRadius;
    static float maxX, maxY, maxZ;
    static int maxDivison;
    static float smallest_radii;
    static grid* cell;
    static particle* particles;

public:


    void data(int n, float Vel, float Radius, float X, float Y, float Z);

    static particle* particleArray();

    // this the radii math which checks whether the particles are actually hitting even though when the cells overlap

    static bool radiiMath(vec3 position1, vec3 position2, float radius1, float radius2) ;

    // this is the actual 2D collision math

    //conservation of linear moment is being used and here the velocities are passed by reference
    static void collisionMath(float ds, float m1, float m2, float* v1, float* v2, vec3 position1, vec3 position2) ;

    // this function creates a virtual grid in the RAM 
    void gridAllocator();

    // this function wipes clean all the data registered in each cell
    void cleanGrid();

    // as we have dynamically allocated memory here , so we need to delete the virtual grid in order prevent memory leak using this function
    void deleteGrid();
};


