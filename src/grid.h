#ifndef GRID_H_
#define GRID_H_

//! \file grid.h

#include <cmath>
#include <string>
#include <vector>
#include <cstdio>

#include "io.h"

class Grid {
public:

    Grid (double depth, double delz0, double delzfrac, double delzmax);

    //!gets number of cells
    double get_n () { return(n); }
    //!gets length/depth of the domain (m)
    double get_dep () { return(dep); }
    //!gets vector of cell edge coordinates (m)
    const std::vector<double> get_ze () const { return(ze); }
    //!gets arravectory of cell center coordinates (m)
    const std::vector<double> get_zc () const { return(zc); }
    //!gets arrayvector of  cell width (m)
    const std::vector<double> get_delz () const { return(delz); }
    //!gets arrayvector of  cell widths used for stability calculations (m)
    const std::vector<double> get_delze () const { return(delze); }
    //!gets arrayvector of factors for cell edge values
    const std::vector<double> get_vefac () const { return(vefac); }
    //!gets arrayvector of factors for cell edge gradients
    const std::vector<double> get_gefac () const { return(gefac); }

    //!writes grid arrays into a directory as binary files
    void save (std::string dirout);

private:

    //!number of cells
    long n;
    //!length of the domain (m)
    double dep;
    //!cell edge coordinates (m)
    std::vector<double> ze;
    //!cell center coordinates (m)
    std::vector<double> zc;
    //!cell width (m)
    std::vector<double> delz;
    //!cell widths used for stability calculations (m)
    std::vector<double> delze;
    //!factors for cell edge values
    std::vector<double> vefac;
    //!factors for cell edge gradients
    std::vector<double> gefac;

    void grid_edges(double depth, double delz0, double delzfrac,
                    double delzmax, std::vector<double> &ze);

};

#endif
