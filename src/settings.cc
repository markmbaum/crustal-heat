//! \file settings.cc

#include "settings.h"

bool eval_txt_bool (const char *s) {
    if (strcmp(s, "true") == 0 ) return(true);
    return(false);
}

bool cmp (const char *s1, const char *s2) {
    if ( strcmp(s1, s2) == 0 ) return(true);
    return(false);
}

long to_long(const char *val) {
    //convert a string to a long int, handling scientific notation
    return( long(std::atof(val)) );
}

Settings parse_settings ( std::vector< std::vector< std::string > > sv ) {

    Settings s;
    const char *set, *val;

    for (int i=0; i < int(sv.size()); i++) {

        //get the setting and value pair
        set = sv[i][0].c_str();
        val = sv[i][1].c_str();

        //get the setting

        if      ( cmp(set, "depth") ) s.depth = std::atof(val);
        else if ( cmp(set, "delz0") ) s.delz0 = std::atof(val);
        else if ( cmp(set, "delzfrac") ) s.delzfrac = std::atof(val);
        else if ( cmp(set, "delzmax") ) s.delzmax = std::atof(val);
        else if ( cmp(set, "save_grid") ) s.save_grid = eval_txt_bool(val);

        else if ( cmp(set, "tint") ) s.tint = std::atof(val);
        else if ( cmp(set, "tunit") ) s.tunit = std::atof(val);
        else if ( cmp(set, "nsnap") ) s.nsnap = to_long(val);
        else if ( cmp(set, "nmaxout") ) s.nmaxout = to_long(val);
        else if ( cmp(set, "dtfac") ) s.dtfac = std::atof(val);

        else if ( cmp(set, "rho0") ) s.rho0 = std::atof(val);
        else if ( cmp(set, "c0") ) s.c0 = std::atof(val);
        else if ( cmp(set, "k0") ) s.k0 = std::atof(val);
        else if ( cmp(set, "qgeo0") ) s.qgeo0 = std::atof(val);
        else if ( cmp(set, "Tsa") ) s.Tsa = std::atof(val);
        else if ( cmp(set, "Tsb") ) s.Tsb = std::atof(val);
        else if ( cmp(set, "Tsc") ) s.Tsc = std::atof(val);
        else if ( cmp(set, "LH") ) s.LH = std::atof(val);
        else if ( cmp(set, "Tf") ) s.Tf = std::atof(val);
        else if ( cmp(set, "ahcw") ) s.ahcw = std::atof(val);

        else if ( cmp(set, "rho") ) s.rho = eval_txt_bool(val);
        else if ( cmp(set, "c") ) s.c = eval_txt_bool(val);
        else if ( cmp(set, "k") ) s.k = eval_txt_bool(val);
        else if ( cmp(set, "cap") ) s.cap = eval_txt_bool(val);
        else if ( cmp(set, "T") ) s.T = eval_txt_bool(val);
        else if ( cmp(set, "dTdz") ) s.dTdz = eval_txt_bool(val);
        else if ( cmp(set, "q") ) s.q = eval_txt_bool(val);
        else if ( cmp(set, "Tmax") ) s.Tmax = eval_txt_bool(val);
        else if ( cmp(set, "Tmin") ) s.Tmin = eval_txt_bool(val);
        else if ( cmp(set, "Ts") ) s.Ts = eval_txt_bool(val);
        else if ( cmp(set, "qs") ) s.qs = eval_txt_bool(val);
        else if ( cmp(set, "t") ) s.t = eval_txt_bool(val);
        else if ( cmp(set, "tsnap") ) s.tsnap = eval_txt_bool(val);

        else {
            std::cout << "FAILURE: unknown setting in settings file: " << set << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return(s);
}

Settings copy_settings (Settings &b) {

    Settings a;
    //grid
    a.depth = b.depth;
    a.delz0 = b.delz0;
    a.delzfrac = b.delzfrac;
    a.delzmax = b.delzmax;
    a.save_grid = b.save_grid;
    //model
    a.tint = b.tint;
    a.tunit = b.tunit;
    a.nsnap = b.nsnap;
    a.nmaxout = b.nmaxout;
    a.dtfac = b.dtfac;
    //physical
    a.rho0 = b.rho0;
    a.c0 = b.c0;
    a.k0 = b.k0;
    a.qgeo0 = b.qgeo0;
    a.Tsa = b.Tsa;
    a.Tsb = b.Tsb;
    a.Tsc = b.Tsc;
    a.LH = b.LH;
    a.Tf = b.Tf;
    a.ahcw = b.ahcw;
    //trackers and output
    a.rho = b.rho;
    a.c = b.c;
    a.k = b.k;
    a.cap = b.cap;
    a.T = b.T;
    a.dTdz = b.dTdz;
    a.q = b.q;
    a.Tmax = b.Tmax;
    a.Tmin = b.Tmin;
    a.Ts = b.Ts;
    a.qs = b.qs;
    a.t = b.t;
    a.tsnap = b.tsnap;

    return(a);
}
