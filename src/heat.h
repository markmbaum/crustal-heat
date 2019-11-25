#ifndef HEAT_H_
#define HEAT_H_

//! \file heat.h

#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>

#include "io.h"
#include "util.h"
#include "grid.h"
#include "settings.h"

//header file for ODE integrator class
#include "ode_trapz.h"

class Heat : public OdeTrapz {

public:

    //!constructs
    Heat (Grid grid, Settings stgin);

    //!settings structure
    Settings stg;

    //--------------
    //grid variables

    //!number of cells
    const long n;
    //!length/depth of domain (m)
    const double dep;
    //!cell edge coordinates (m)
    const std::vector<double> ze;
    //!cell center coordinates (m)
    const std::vector<double> zc;
    //!cell width (m)
    const std::vector<double> delz;
    //!cell widths used for stability calculations (m)
    const std::vector<double> delze;
    //!factors for cell edge values
    const std::vector<double> vefac;
    //!factors for cell edge gradients
    const std::vector<double> gefac;

    //------------------
    //physical variables

    //static scalar quantities

    //!density of medium (kg/m^3)
    std::vector<double> rho;
    //!specific heat of medium (J/kg*K)
    std::vector<double> c;
    //!thermal conductivity of medium (W/m*K)
    std::vector<double> k;
    //!thermal capacity c*rho (J/m^3*K)
    std::vector<double> cap;

    //dynamic, depth-varying quantities

    //!derivative of temperature w/r/t z
    std::vector<double> dTdz;
    //!fluxes
    std::vector<double> q;

    //---------------
    //other variables

    //maximum stable time step
    double dtmax;

    //--------
    //trackers

    //!time tracker
    std::vector<double> t;
    //!maximum temperature tracker
    std::vector<double> Tmax;
    //!minimum temperature tracker
    std::vector<double> Tmin;
    //!surface temperature tracker
    std::vector<double> Ts;
    //!surface heat flux tracker
    std::vector<double> qs;
    //!snapshot times
    std::vector<double> tsnap;

    //-------------------
    //physical parameters

    //!surface temperature over time (K)
    virtual double f_Ts (double t, double Tsa, double Tsb, double Tsc);
    //!geothermal gradient over time (W/m^2)
    virtual double f_qgeo (double qgeo0, double t);
    //!thermal conductivity over depth (W/m*K)
    virtual double f_k (double k0, double depth);
    //!medium density over depth (kg/m^3)
    virtual double f_rho (double rho0, double depth);
    //!medium specific heat over depth (J/kg*K)
    virtual double f_c (double c0, double depth);

    //------------------
    //physical functions

    //!computes temperature along a linear geothermal temperature profile
    double f_geotherm (double T0, double qgeo, double k, double depth);
    //!computes the flux between two cells
    double f_q (double dTdz, double k);
    //!computes the time derivative of a cell, given fluxes on its sides
    double f_dTdt (double qb, double qt, double cap, double delz);

    //--------------------
    //ODE solver functions

    //!ode function for the integrator
    void ode_fun (double *solin, double *fout);

    //!computes the next time step, based on the maximum diffusivity
    double dt_adapt ();

    //------
    //extras

    //!does extra stuff before starting a solve
    void before_solve ();
    //!does extra stuff after every snap
    void after_snap (std::string dirout, long isnap, double tin);
    //!does extra stuff after every step
    void after_step (double tin);
    //!does extra stuff after integrating
    void after_solve ();
};

#endif
