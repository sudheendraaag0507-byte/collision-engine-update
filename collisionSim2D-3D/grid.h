#pragma once
#include <iostream>

class particle ;
class general ;

class grid :general {
private:
    // this is a list dedicated for each cell for knowing how many balls had filled the same cell
    std::list<int> box;
    // this is a list dedicated for the entire grid system in order to prevent uneccesary provoking of collision math for overlapping of cells between the same particles
    static list<int> newBall;

public:
    // this function clears the list after drawing each and every ball
    void cleanlist() ;
    // fills the cells for a given particles
    void fill(int a, int k, int j) ;

    void clean() ;
};