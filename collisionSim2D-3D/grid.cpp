#include <iostream>
#include "include/boilerPlate.h"
#include "general.h"
#include "particle.h"
#include "grid.h"

    // this is where are the data regarding the grid and cells live

    // this function clears the list after drawing each and every ball
    void grid::cleanlist() {
        newBall.clear();
    }
    // fills the cells for a given particles
    void grid::fill(int a, int k, int j) {

        if (box.size() == 0) {
            box.push_back(a);
        }

        else {
            bool is_hitting;
            for (int i : box) {
                if (find(newBall.begin(), newBall.end(), i) == newBall.end()) {
                    newBall.push_back(i);
                    is_hitting = radiiMath(particles[a].pos(), particles[i].pos(), particles[a].rad(), particles[i].rad());


                    if (is_hitting) {


                        vec3 Prel = particles[a].pos() - particles[i].pos();


                        vec3 Vrel = (particles[a].vel()) - (particles[i].vel());
                        //cout << Vrel.x << Vrel.y << Vrel.z << "\n";

                        float Ds = dot(Vrel, Prel);

                        //if (Ds > 0)
                        //cout << Ds << " " << ((particles[a].vel()) - (particles[i].vel())).x << " " << ((particles[a].vel()) - (particles[i].vel())).y << " " << a << " " << i << "<------this is already got hit\n";
                        //else cout << Ds <<" " << ((particles[a].vel()) - (particles[i].vel())).x << " " << ((particles[a].vel()) - (particles[i].vel())).y << " grid no :" << k << " " << j << " " << a << " " << i << "<-------this about to !!  \n";

                        collisionMath(Ds, particles[a].m(), particles[i].m(), value_ptr(particles[a].vel()), value_ptr(particles[i].vel()), particles[a].pos(), particles[i].pos());
                    }
                }
            }
            box.push_back(a);
        }

    }

    void grid::clean() {
        box.clear();
    }

grid* general::cell = nullptr;
list<int> grid::newBall = { -1 };