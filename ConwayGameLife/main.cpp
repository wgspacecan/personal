
// Game of Life - Controller

#include "conway_life.hpp"

// - settings -
bool import_file = false;
std::string filename = "aubs";
unsigned int timestep = 99;
unsigned int random_size_x = 40;
unsigned int random_size_y = 40;
// - end settings -

int main() {
    Conway::World world;
    world.timestep = timestep;
    if (import_file) world.import_world_file(filename);
    else world.random_world(random_size_x, random_size_y);
    world.my_sleep(timestep);
    while (!world.game_over) {
        world.cycle();
        world.my_sleep(timestep);
    }
}
