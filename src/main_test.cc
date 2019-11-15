#include <string>
#include <vector>
#include <iostream>

#include "io.h"
#include "grid.h"
#include "settings.h"
#include "heat.h"

//!driver
int main (int argc, char **argv) {

    if ( argc != 2 )
        print_exit("crustal_heat_test.exe must be given a command line argument, the path to an output directory.");

    //store output directory
    std::string dirout = argv[1];

    //make a settings structure
    Settings stg;
    stg.depth = 2;
    stg.T = true;
    stg.nsnap = 2;
    stg.qgeo0 = 0;
    stg.tint = 0.02;

    stg.Tsc = 1e-100;
    stg.delz0 = 0.025;
    double delzf = 0.0005;
    double frac = 0.75;
    int count = 0;
    std::vector<double> delz;
    while ( stg.delz0 > delzf ) {
        //store the grid spacing
        delz.push_back( stg.delz0 );
        //create a grid
        Grid grid(stg.depth, stg.delz0, stg.delzfrac, stg.delzmax);
        write_double(dirout + "/" + int_to_string(count) + "_zc", grid.get_zc());
        //create an integrator
        Heat heat(grid, stg);
        heat.set_name(int_to_string(count));
        //solve
        heat.solve_fixed(stg.tint*stg.tunit, stg.tint*1e-12, stg.nsnap, dirout.c_str());
        //increment counter
        count++;
        //reduce grid spacing
        printf("%2d) integration with h = %g complete\n", count, stg.delz0);
        stg.delz0 *= frac;
    }

    write_double(dirout + "/delz", delz);

    return(0);
}
