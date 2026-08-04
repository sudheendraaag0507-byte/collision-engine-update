#include <iostream>
#include "boilerPlate.h"
#include "general.h"
#include "particle.h"
#include "grid.h"

// Static definitions
grid* general::cell = nullptr;

/*
 * These static variables remember collisions between two balls.
 * Using atomic variables instead of locks prevents crashes, offering higher throughput and cleaner execution.
 */
std::atomic<bool> grid::is_checked = false;
std::atomic<int> grid::ballNumber = 1;

// Fills the cells for a given particle and resolves spatial grid collisions
void grid::fill(int a) {
    if (ballNumber != a) {
        is_checked.exchange(false);
        ballNumber.exchange(a);
    }

    if (box.empty()) {
        box.push_back(a);
    }
    else {
        if (!is_checked.exchange(true)) {
            bool is_hitting = general::radiiMath(
                general::particles[a].pos(),
                general::particles[box.front()].pos(),
                general::particles[a].rad(),
                general::particles[box.front()].rad()
            );

            if (is_hitting) {
                vec3 Prel = general::particles[a].pos() - general::particles[box.front()].pos();
                vec3 Vrel = general::particles[a].vel() - general::particles[box.front()].vel();

                float Ds = dot(Vrel, Prel);

                general::collisionMath(
                    Ds,
                    general::particles[a].m(),
                    general::particles[box.front()].m(),
                    value_ptr(general::particles[a].vel()),
                    value_ptr(general::particles[box.front()].vel()),
                    general::particles[a].pos(),
                    general::particles[box.front()].pos()
                );
            }

            box.push_back(a);
        }
    }
}

// Clears the particle index buffer in the spatial cell
void grid::clean() {
    box.clear();
}