#include <cstdio>
#include <string>
#include <vector>
#include <iostream>

#include "omp.h"

#include "io.h"
#include "util.h"
#include "grid.h"
#include "settings.h"
#include "impact_layer.h"

//!model driver
int main (int argc, char **argv) {

    if ( argc != 3 )
        print_exit("crustal_heat must be given two command line arguments\n  1. path to settings file\n  2. path to output directory");

    //domain size and integration time
    double depfac = 6; //multiple of impact layer depth for total domain depth
    double timfac = 12; //multiple of thermal time scale for total integration time
    int ncell = 240; //number of cells to put in the impact layer

    //parameter ranges
    std::vector<double> deplayer = logspace(0, 2.69897, 10);
    std::vector<double> Tsinterp = linspace(0, 2, 3); //false and true
    std::vector<double> Tlayer = linspace(400, 1200, 5);
    double Tbelow = 220.0;
    std::string fnTs = "1bar100km";

    //binary files surface temperature directory
    std::string dirTs = "atmospheric-temperature/reformatted";

    //store output directory
    std::string dirout = argv[2];

    //read default settings
    Settings stg = parse_settings(read_values(argv[1]));

    //allocate parameter table
    long nparam = long(deplayer.size()*Tsinterp.size()*Tlayer.size());
    double **param = new double*[nparam];
    for (long i=0; i<nparam; i++) param[i] = new double[3];

    //fill and write parameter table
    std::string fn = dirout + "/trials.csv";
    check_file_write(fn.c_str());
    FILE *ofile = fopen(fn.c_str(), "w");
    fprintf(ofile, "trial,layer thickness (m),impact atmos temp?,initial layer temperature (K)\n");
    long unsigned count = 0;
    for (long i=0; i<long(deplayer.size()); i++) {
        for (long j=0; j<long(Tsinterp.size()); j++) {
            for (long k=0; k<long(Tlayer.size()); k++) {
                //fill a row of the table
                param[count][0] = deplayer[i];
                param[count][1] = Tsinterp[j];
                param[count][2] = Tlayer[k];
                //write to file
                fprintf(ofile, "%li,%g,%g,%g\n",
                    count,
                    param[count][0],
                    param[count][1],
                    param[count][2]);
                count++;
            }
        }
    }
    fclose(ofile);
    printf("parameter table written to: %s\n", fn.c_str());

    printf("beginning parallel integrations with %d threads\n", omp_get_max_threads());
    printf("%li trials to integrate\n", nparam);
    #pragma omp parallel for schedule(dynamic)
    for (long i=0; i<nparam; i++) {
        //create a grid
        Grid grid(depfac*param[i][0], param[i][0]/double(ncell), 1, 1e9);
        //write the cell coordinates
        write_double(dirout + "/" + int_to_string(i) + "_zc", grid.get_zc());
        //create a solver
        ImpactLayer heat(grid, stg, Tbelow, param[i][2], param[i][0], dirTs, fnTs, bool(param[i][1]));
        heat.set_name(int_to_string(i));
        heat.set_quiet(true);
        //integration time
        double tint = timfac*(param[i][0]*param[i][0])/(heat.k[0]/heat.cap[0]);
        //integrate
        heat.solve_adaptive(tint, tint*1e-12, stg.nsnap, dirout.c_str());
        printf("  trial %li finished\n", i);
    }
    printf("all trials complete\n\n");

    for (long i=0; i<nparam; i++) delete [] param[i];
    delete [] param;

    return(0);
}
