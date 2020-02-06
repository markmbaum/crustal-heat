#include <cstdio>
#include <string>
#include <vector>
#include <iostream>

#include "omp.h"

#include "io.h"
#include "util.h"
#include "grid.h"
#include "heat.h"
#include "settings.h"

//!model driver
int main (int argc, char **argv) {

    //output csv
    std::string fn;
    FILE *ofile;
    //indices & counters
    long long unsigned i,ia,ib,ic,id,count,nparam;
    //parameter vectors
    std::vector<double> k0, qgeo0, Tsa, Tsb;
    //parameter range specifications
    double k0a = 1, k0b = 7; int k0n = 7;
    double qgeo0a = 0.01, qgeo0b = 0.1; int qgeo0n = 10;
    double Tsaa = 200, Tsab = 260; int Tsan = 30;
    double Tsba = 280, Tsbb = 320; int Tsbn = 20;
    //parameter table
    double **param;

    if ( argc != 3 )
        print_exit("thaw_times must be given two command line arguments\n  1. path to default settings file\n  2. path to output directory");

    //store output directory
    std::string dirout = argv[2];

    //read settings
    Settings stg = parse_settings(read_values(argv[1]));

    //create grid
    Grid grid(stg.depth, stg.delz0, stg.delzfrac, stg.delzmax);
    if ( stg.save_grid )
        grid.save(dirout);

    //create parameter ranges
    k0 = linspace(k0a, k0b, k0n);
    qgeo0 = linspace(qgeo0a, qgeo0b, qgeo0n);
    Tsa = linspace(Tsaa, Tsab, Tsan);
    Tsb = linspace(Tsba, Tsbb, Tsbn);

    //allocate parameter table
    nparam = k0.size()*qgeo0.size()*Tsa.size()*Tsb.size();
    param = new double*[nparam];
    for (i=0; i<nparam; i++) param[i] = new double[4];

    //fill parameter table and write parameter combinations to a file
    fn = dirout + "/trials.csv";
    check_file_write(fn.c_str());
    ofile = fopen(fn.c_str(), "w");
    fprintf(ofile, "trial,k0,qgeo0,Tsa,Tsb\n");
    count = 0;
    for (ia=0; ia<k0.size(); ia++) {
        for (ib=0; ib<qgeo0.size(); ib++) {
            for (ic=0; ic<Tsa.size(); ic++) {
                for (id=0; id<Tsb.size(); id++) {
                    //fill in row of table
                    param[count][0] = k0[ia];
                    param[count][1] = qgeo0[ib];
                    param[count][2] = Tsa[ic];
                    param[count][3] = Tsb[id];
                    //write to file
                    fprintf(ofile, "%lli,%g,%g,%g,%g\n",
                        count,
                        param[count][0],
                        param[count][1],
                        param[count][2],
                        param[count][3]);
                    //increment
                    count++;
                }
            }
        }
    }
    //close the csv
    fclose(ofile);
    printf("parameter table written to: %s\n", fn.c_str());

    printf("beginning parallel integrations with %d threads\n", omp_get_max_threads());
    #pragma omp parallel for schedule(dynamic)
    for (long long unsigned i=0; i<nparam; i++) {
        //copy settings
        Settings stgi = copy_settings(stg);
        //edit parameters
        stgi.k0 = param[i][0];
        stgi.qgeo0 = param[i][1];
        stgi.Tsa = param[i][2];
        stgi.Tsb = param[i][3];
        //create solver
        Heat heat(grid, stgi);
        heat.set_quiet(true);
        heat.set_name(int_to_string(i));
        //integrate
        double tint = stgi.tint*stgi.tunit;
        heat.solve_adaptive(tint, tint*1e-12, stgi.nsnap, dirout.c_str());
    }

    for (i=0; i<nparam; i++) delete [] param[i];
    delete [] param;

    return(0);
}
