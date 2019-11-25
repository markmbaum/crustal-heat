#include <string>
#include <vector>
#include <iostream>

#include "io.h"
#include "grid.h"
#include "settings.h"
#include "heat.h"

//!model driver
int main (int argc, char **argv) {

    if ( argc != 3 )
        print_exit("crustal_heat must be given two command line arguments\n  1. path to settings file\n  2. path to output directory");

    //store output directory
    std::string dirout = argv[2];

    //read settings
    Settings stg = parse_settings(read_values(argv[1]));

    //create grid
    Grid grid(stg.depth, stg.delz0, stg.delzfrac, stg.delzmax);
    if ( stg.save_grid )
        grid.save(dirout);

    //create solver
    Heat heat(grid, stg);

    //integrate
    double tint = stg.tint*stg.tunit;
    heat.solve_adaptive(tint, 1e-12*tint, stg.nsnap, dirout.c_str());

    return(0);
}
