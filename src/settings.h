#ifndef SETTINGS_H_
#define SETTINGS_H_

//! \file settings.h

#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <cstdlib>

//!container struct for all the settings variables needed for a BousThermModel run
class Settings {
public:

    //-------------------------------------
    //grid settings

    //!domain depth (m)
    double depth = 1.0;
    //!surface cell width (m)
    double delz0 = 0.01;
    //!fraction increase for deeper neighbor cell
    double delzfrac = 1.0;
    //!maximum cell width (m)
    double delzmax = 1.0;
    //!whether to write grid files
    bool save_grid = false;

    //-------------------------------------
    //model set up and integration settings

    //!duration of solve
    double tint = 1.0;
    //!time unit to use (number of seconds)
    double tunit = 1.0;
    //!number of snaps to take
    long nsnap = 5;
    //!maximum length of output vectors (subsampled to accomodate)
    long nmaxout = 100;
    //!safety factor for stable time step
    double dtfac = 0.9;

    //-------------------------------------
    //physical parameters

    //!medium density constant (kg/m^3)
    double rho0 = 1.0;
    //!medium specific heat constant (J/kg*K)
    double c0 = 1.0;
    //!medium thermal conductivity constant (W/m*K)
    double k0 = 1.0;
    //!geothermal heat flux (W/m^2)
    double qgeo0 = 1.0;
    //!first surface temperature parameter
    double Tsa = 0.0;
    //!second surface temperature parameter
    double Tsb = 1.0;
    //!third surface temperature parameter
    double Tsc = 1.0;

    //-------------------------------------
    //trackers and output

    //!whether to write density profile before integrating
    bool rho = false;
    //!whether to write specific heat profile before integrating
    bool c = false;
    //!whether to write conductivity profile before integrating
    bool k = false;
    //!whether to write heat capacity before integrating
    bool cap = false;
    //!whether to write temperature profile when snapping
    bool T = false;
    //!whether to write thermal gradient when snapping
    bool dTdz = false;
    //!whether to write thermal flux when snapping
    bool q = false;
    //!whether to track maximum temperature
    bool Tmax = false;
    //!whether to track minimum temperature
    bool Tmin = false;
    //!whether to track surface temperature
    bool Ts = false;
    //!whether to track surface heat flux
    bool qs = false;
    //!wheter to track time step times
    bool t = false;
    //!wheter to track time snap times
    bool tsnap = false;

};

//!converts a string into a bool if it's supposed to be "true" or "false"
bool eval_txt_bool (const char *s);

//!abbreviates strcmp
bool cmp (const char *s1, const char *s2);

//!converts a character to an integet
long to_long(const char *val);

//!parses a settings file and returns it in a Settings structure
/*!
\param[in] sv vector of vectors of strings from read_values_file()
*/
Settings parse_settings ( std::vector< std::vector< std::string > > sv );

//!constructs a copy of another Settings object
/*!
\param[in] b the Settings object to copy
*/
Settings copy_settings (Settings &b);

#endif
