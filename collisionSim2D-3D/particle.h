#pragma once
#include <iostream>
class grid ;
class general ;

class particle :general {
protected:
    float radius = 0.0f;
    float mass = 0.0f;
    vec3 position = vec3(0.0f, 0.0f, 0.0f);
    vec3 velocity = vec3(0.0f, 0.0f, 0.0f);
    int divisonX = 0, divisonY = 0 , divisonZ = 0;
    int ballNumber = 0;

public:

    vec3& vel();

    float m() ;

    float rad() ;

    vec3& pos() ;

    // this function calculate the cell in which the particle's centre belong to 
    void divisionCalculator() ;

    // this function fills the individual cells present inside the grid based on the balls position
    void painter();

    void dataCollector(int i) ;

    // this function defines the containment 
    void changeVelocity() ;

    void changePosition() ;

};