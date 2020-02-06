#ifndef IMPACT_LAYER_H_
#define IMPACT_LAYER_H_

//! file impact_layer.h

#include <string>

#include "grid.h"
#include "settings.h"
#include "heat.h"

class ImpactLayer : public Heat {
public:

    //!constructs
    ImpactLayer (
        Grid grid,
        Settings stgin,
        double Tbelow_,
        double Tlayer,
        double deplayer,
        std::string dirTs,
        std::string fnTs,
        int Tsmode_);
    //!destructs
    ~ImpactLayer ();

    //!temperature below the hot layer
    double Tbelow;

    //!length of surface temperature arrays
    long nTs;
    //!time for surface temperature interpolation
    double *tTs;
    //!surface temperatures for interpolation
    double *Ts;
    //!process to use for surface temperature
    /*!
    0 - constant surface temperature of 220 K
    1 - sigma*T^4 radiation to space
    2 - interpolation of temperature time series from file (dirTs/fnTs)
    */
    int Tsmode;

    //!initializes an upper layer with a uniform temperature
    void init_layer (double Ts, double Tlayer, double depth);

    //!computes a surface temperature for Stefan-Boltzmann radiative flux
    /*!
    \param[in] Tend temperature of the surface cell
    \param[in] k thermal conductivity of the surface cell
    */
    double f_Stefan_Boltzmann (double Tend, double k, double delz);

    //!surface temperature over time (K)
    double f_Ts (double t, double Tsa, double Tsb, double Tsc);

};

#endif
