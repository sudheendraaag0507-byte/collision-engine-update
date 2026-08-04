#pragma once

#include <iostream>
#include <vector>
#include <atomic>

class particle;
class general;

class grid {
    friend class particle;
    friend class general;

private:
    // Tracks which particles occupy this grid cell
    std::vector<int> box;

    // Prevents redundant collision checks when particles overlap multiple shared cells
    static std::atomic<bool> is_checked;
    static std::atomic<int> ballNumber;

public:
    // Clears the particle list after processing each frame
    void cleanlist();

    // Registers a particle ID into this cell
    void fill(int a);

    // Cleans cell state
    void clean();
};