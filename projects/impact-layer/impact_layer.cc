//! \file impact_layer.cc

#include "impact_layer.h"

ImpactLayer::ImpactLayer (
    Grid grid,
    Settings stgin,
    double Tbelow_,
    double Tlayer,
    double deplayer,
    std::string dirTs,
    std::string fnTs,
    int Tsmode_) :
        Heat (grid, stgin) {

    //store surface temp before hot layer emplacement
    Tbelow = Tbelow_;
    //store surface temperature mode
    Tsmode = Tsmode_;

    //initialize the hot upper layer
    init_layer(Tbelow, Tlayer, deplayer);

    //read in surface temperature files for interpolation
    if ( Tsmode == 2 ) {
        nTs = read_one_long(dirTs, "n.txt");
        tTs = new double[nTs];
        read_double(dirTs, "time", tTs, nTs);
        Ts = new double[nTs];
        read_double(dirTs, fnTs, Ts, nTs);
    }
}

ImpactLayer::~ImpactLayer () {
    if ( Tsmode == 2 ) {
        delete [] tTs;
        delete [] Ts;
    }
}

void ImpactLayer::init_layer (double Tbelow, double Tlayer, double deplayer) {

    for (long i=0; i<n; i++) {
        if ( -zc[i] < deplayer ) {
            //inside the hot layer
            this->set_sol(i, Tlayer);
        } else {
            //below the hot layer
            this->set_sol(i,
                f_geotherm(Tbelow,
                    f_qgeo(stg.qgeo0, 0),
                    f_k(stg.k0, -zc[i] - deplayer),
                    -zc[i] - deplayer
                )
            );
        }
    }
}

double ImpactLayer::f_Stefan_Boltzmann (double Tend, double k, double delz) {
    //Stefan-Boltzmann constant, SI units
    double sigma = 5.67e-8;
    //Newton solve for the appropriate surface temperature
    double Ts0, Ts1, f, df;
    Ts0 = INFINITY;
    Ts1 = Tend;
    while ( fabs(Ts0 - Ts1)/fabs(Ts1) > 1e-9 ) {
        //store previous estimate
        Ts0 = Ts1;
        //evaluate the function equating surface flux w/ Stefan-Boltzmann
        f = delz*(sigma*(Ts1*Ts1*Ts1*Ts1) - 114) + k*Ts1 - k*Tend;
        //evaluate its derivative
        df = 4*delz*sigma*(Ts1*Ts1*Ts1) + k;
        //update
        Ts1 -= f/df;
    }
    //return the final surface temperature value
    return(Ts1);
}

double ImpactLayer::f_Ts (double t, double Tsa, double Tsb, double Tsc) {
    //normal parameters aren't needed
    (void)Tsa; (void)Tsb; (void)Tsc;
    //switch on the input surface temperature mode
    switch ( Tsmode ) {
        case 0:
            return( 220.0 );
            break;
        case 1:
            return( f_Stefan_Boltzmann(get_sol(n-1), k[n], delz[n-1]/2) );
            break;
        case 2:
            return( interp(tTs, Ts, t, nTs) );
            break;
        default:
            printf("Erroneous Tsmode value\n");
            return(NAN);
    }
}
