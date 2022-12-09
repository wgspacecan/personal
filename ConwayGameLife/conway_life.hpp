// Conway's Game of Life
// Code written by William G.  Copyright (c) 2016 William. All rights reserved.

// TODO:
// - add seed option
// - move Life class into World class and main file

#ifndef _CONWAY_LIFE_
#define _CONWAY_LIFE_

#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h> // rand, system
#include <fstream>
#include <string>

//#define CL_DEBUG
#define CL_FANCY

namespace Conway {

    class Cell {
    private:
        bool alive;
    public:
        Cell() {
            alive = false;
        }
        // input: neighbour count
        void cycle(unsigned int in_nc) {
            if (alive) {
                if (in_nc < 2 || in_nc > 3) alive = false;
                else alive = true;
            } else {
                if (in_nc == 3) alive = true;
            }
        }
        bool set_state(bool in_s) {
            alive = in_s;
        }
        bool return_state() {
            return alive;
        }
    };

    class World {
    private:
        std::vector <std::vector <Cell> > cells;
        std::vector <std::vector <bool> > last_last_state;
        std::vector <std::vector <bool> > last_state;
        unsigned int cycle_count;
        /// Functions
        void export_world_file() {
            std::ofstream file;
            file.open("last_run_inital.gol", std::ofstream::out | std::ofstream::trunc);
            if (file.is_open()) {
                for (std::size_t i=0; i<last_state.size(); ++i) {
                    for (std::size_t j=0; j<last_state.at(i).size(); ++j) {
                        if (last_state.at(i).at(j)) file << "0";
                        else file << "-";
                    }
                    if (i != last_state.size()-1) file << "\n";
                }
            } else std::cout << "ERROR: can not open file" << std::endl;
            file.close();
        }
        void update_state() {
#ifdef CL_DEBUG
            std::cout << "DEBUG: update_state()" << std::endl;
#endif
            last_last_state = last_state;
            last_state.clear();
            for (std::size_t i=0; i<cells.size(); ++i) {
                std::vector <bool> t;
                for (std::size_t j=0; j<cells.at(i).size(); ++j) {
                    t.push_back(cells.at(i).at(j).return_state());
                }
                last_state.push_back(t);
            }
            if (cycle_count == 0) export_world_file();
        }
        unsigned int count_neighbours(unsigned int in_i, unsigned int in_j) {
#ifdef CL_DEBUG
            std::cout << "DEBUG: count_neighbours()" << std::endl;
#endif
            unsigned int t_count = 0;
            for (std::size_t i=in_i-1; i<in_i+2; ++i) {
                for (std::size_t j=in_j-1; j<in_j+2; ++j) {
                    if (i == in_i && j == in_j) {}
                    else {
                        if (i >= 0 && j >= 0) {
                            if (i < last_state.size() && j < last_state.at(0).size()) {
                                if (last_state.at(i).at(j) == true) t_count += 1;
                            }
                        }
                    }
                }
            }
            return t_count;
        }
        void cycle_cells() {
#ifdef CL_DEBUG
            std::cout << "DEBUG: cycle_cells()" << std::endl;
#endif
            for (std::size_t i=0; i<cells.size(); ++i) {
                for (std::size_t j=0; j<cells.at(i).size(); ++j) {
                    cells.at(i).at(j).cycle(count_neighbours(i, j));
                }
            }
            ++cycle_count;
        }
        void display_world() {
#ifdef CL_DEBUG
            std::cout << "DEBUG: display_world()" << std::endl;
#endif
#ifdef CL_FANCY
            system("clear");
            std::cout << std::endl;
            std::cout << "Game of Life" << std::endl;
            std::cout << "cycle: " << cycle_count << std::endl;
            std::cout << std::endl;
#endif
            for (std::size_t i=0; i<last_state.at(0).size(); ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            for (std::size_t i=0; i<last_state.size(); ++i) {
                for (std::size_t j=0; j<last_state.at(i).size(); ++j) {
                    if (last_state.at(i).at(j) == true) {
                        std::cout << "o";
                    } else {
                        std::cout << " ";
                    }
                }
                std::cout << "|" << std::endl;
            }
            for (std::size_t i=0; i<last_state.at(0).size(); ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            if (game_over) {
                std::cout << std::endl;
                std::cout << "Game Over" << std::endl;
                std::cout << std::endl;
            }
        }
        void check_done() {
            if (last_state == last_last_state) {
                game_over = true;
                --cycle_count;
            }
        }
    public:
        unsigned int timestep; // micro seconds
        bool game_over;
        World() {
            srand(time(NULL));
            timestep = 1000;
            game_over = false;
            cycle_count = 0;
        }
        void my_sleep(unsigned int in_ts) {
            struct timespec req = {0};
            req.tv_sec = 0;
            req.tv_nsec = in_ts * 1000000L;
            nanosleep(&req, (struct timespec *)NULL);
        }
        void import_world_file(std::string in_name) {
            std::vector <std::vector <bool> > t_w;
            std::ifstream file;
            std::string t_name = "saved_worlds/" + in_name + ".gol";
            file.open(t_name, std::ifstream::in);
            if (file.is_open()) {
                std::string line;
                while (getline(file, line)) {
                    std::vector <bool> t;
                    for (std::size_t i=0; i<line.size(); ++i) {
                        if (line.at(i) == '0') t.push_back(true);
                        else if (line.at(i) == '-') t.push_back(false);
                        else {
                            std::cout << "ERROR: world import error" << std::endl;
                            game_over = true;
                        }
                    }
                    t_w.push_back(t);
                }
            } else std::cout << "ERROR: can not open file" << std::endl;
            file.close();
            set_world(t_w);
        }
        void random_world(unsigned int in_x, unsigned int in_y) {
#ifdef CL_DEBUG
            std::cout << "DEBUG: random_world()" << std::endl;
#endif
            cells.clear();
            for (std::size_t i=0; i<in_x; ++i) {
                std::vector <Cell> t;
                for (std::size_t j=0; j<in_y; ++j) {
                    Cell c;
                    if ((double)rand()/RAND_MAX > 0.4)
                        c.set_state(true);  // add random selection FIX
                    else c.set_state(false);
                    t.push_back(c);
                }
                cells.push_back(t);
            }
            update_state();
            display_world();
        }
        void set_world(std::vector <std::vector <bool> > in_w) {
#ifdef CL_DEBUG
            std::cout << "DEBUG: set_world()" << std::endl;
#endif
            cells.clear();
            for (std::size_t i=0; i<in_w.size(); ++i) {
                std::vector <Cell> t;
                for (std::size_t j=0; j<in_w.at(i).size(); ++j) {
                    Cell c;
                    c.set_state(in_w.at(i).at(j));
                    t.push_back(c);
                }
                cells.push_back(t);
            }
            update_state();
            display_world();
        }
        void cycle() {
            cycle_cells();
            update_state();
            check_done();
            display_world();
        }
    };

}

#endif
