//! \file heat.cc

#include "heat.h"

Heat::Heat (Grid grid, Settings stgin) :
    OdeTrapz (grid.get_n()),
    stg (copy_settings(stgin)),
    n     (grid.get_n()),
    dep   (grid.get_dep()),
    ze    (grid.get_ze()),
    zc    (grid.get_zc()),
    delz  (grid.get_delz()),
    delze (grid.get_delze()),
    vefac (grid.get_vefac()),
    gefac (grid.get_gefac()) {

    long i;

    //set the name of the object
    this->set_name("heat");
    //turn on silent snapping
    this->set_silent_snap(true);

    //------------------
    //physical variables

    //density of medium
    for (i=0; i<n; i++) rho.push_back( f_rho(stg.rho0, -zc[i]) );
    //specific heat of medium
    for (i=0; i<n; i++) c.push_back( f_c(stg.c0, -zc[i]) );
    //thermal conductivity of medium
    for (i=0; i<n+1; i++) k.push_back( f_k(stg.k0, -ze[i]) );
    //thermal capacity
    for (i=0; i<n; i++) cap.push_back( rho[i]*c[i] );

    //derivative of moisture w/r/t z
    dTdz.resize(n+1);
    //fluxes
    q.resize(n+1);

    //------------------
    //initial condition

    for (i=0; i<n; i++)
        set_sol(i,
            f_geotherm(
                f_Ts(0, stg.Tsa, stg.Tsb, stg.Tsc),
                f_qgeo(stg.qgeo0, 0),
                f_k(stg.k0, 0),
                -zc[i]
            )
        );

    //---------------
    //maximum stable time step

    double dt, tem;
    dtmax = INFINITY;
    for (long i=0; i<n+1; i++) {
        //get the appropriate (maximum) capacity
        if ( i == 0 ) {
            tem = cap[0];
        } else if ( i == n ) {
            tem = cap[n-1];
        } else {
            tem = cap[i] > cap[i-1] ? cap[i] : cap[i-1];
        }
        //compute stable time step
        dt = delze[i]*delze[i]/(2.0*k[i]/tem);
        if ( dtmax > dt )
            dtmax = dt;
    }
}

//------------------------------------------------------------------------------
//physical parameters

double Heat::f_Ts (double t, double Tsa, double Tsb, double Tsc) {
    return(
        Tsa + (Tsb - Tsa)*(1.0 - exp(-t/Tsc))
    );
}

double Heat::f_qgeo (double qgeo0, double t) {
    (void)t;
    return(
        qgeo0
    );
}

double Heat::f_k (double k0, double depth) {
    (void)depth;
    return(
        k0
    );
}

double Heat::f_rho (double rho0, double depth) {
    (void)depth;
    return(
        rho0
    );
}

double Heat::f_c (double c0, double depth) {
    (void)depth;
    return(
        c0
    );
}

//------------------------------------------------------------------------------
//physical functions

double Heat::f_geotherm (double T0, double qgeo, double k, double depth) {
    return(
        T0 + qgeo*depth/k
    );
}

double Heat::f_q (double dTdz, double k) {
    return(
        -dTdz*k
    );
}

double Heat::f_dTdt (double qb, double qt, double cap, double delz) {
    return(
        ((qb - qt)/cap)/delz
    );
}

//------------------------------------------------------------------------------
//ODE solver functions

void Heat::ode_fun (double *solin, double *fout) {

    //index
    long i;
    //alias
    double *Tin = solin;
    double *dTdt = fout;

    //cell edge gradients and fluxes
    dTdz[0] = -f_qgeo(stg.qgeo0, this->get_t())/k[0];
    q[0] = f_q(dTdz[0], k[0]);
    for (i=1; i<n; i++) {
        dTdz[i] = gefac[i]*(Tin[i] - Tin[i-1]);
        q[i] = f_q(dTdz[i], k[i]);
    }
    dTdz[n] = (f_Ts(this->get_t(), stg.Tsa, stg.Tsb, stg.Tsc) - Tin[n-1])/(delz[n-1]/2);
    q[n] = f_q(dTdz[n], k[n]);

    //time derivatives
    for (i=0; i<n; i++)
        dTdt[i] = f_dTdt(q[i], q[i+1], cap[i], delz[i]);

    //printf("%g\n", get_t());
}

double Heat::dt_adapt () {
    return(stg.dtfac*dtmax);
}

//------------------------------------------------------------------------------
//extras

void Heat::before_solve () {
    std::string name = this->get_name();
    std::string dirout = this->get_dirout();
    if ( stg.rho )
        write_double(dirout + "/" + name + "_rho", rho);
    if ( stg.c )
        write_double(dirout + "/" + name + "_c", c);
    if ( stg.k )
        write_double(dirout + "/" + name + "_k", k);
    if ( stg.cap )
        write_double(dirout + "/" + name + "_cap", cap);
}

void Heat::after_snap (std::string dirout, long isnap, double tin) {
    std::string name = this->get_name();
    std::string i = int_to_string(isnap);
    if ( stg.T )
        write_double(dirout + "/" + name + "_T_" + i, this->get_sol(), n);
    if ( stg.dTdz )
        write_double(dirout + "/" + name + "_dTdz_" + i, dTdz);
    if ( stg.q )
        write_double(dirout + "/" + name + "_q_" + i, q);
    if ( stg.tsnap )
        tsnap.push_back( tin );
}

void Heat::after_step (double tin) {
    double *T = this->get_sol();
    if ( stg.Tmax )
        Tmax.push_back( max(T, n) );
    if ( stg.Tmin )
        Tmin.push_back( min(T, n) );
    if ( stg.Ts )
        Ts.push_back( f_Ts(tin, stg.Tsa, stg.Tsb, stg.Tsc) );
    if ( stg.qs )
        qs.push_back( f_q((f_Ts(tin, stg.Tsa, stg.Tsb, stg.Tsc) - T[n-1])/(delz[n-1]/2), k[0]) );
    if ( stg.t )
        t.push_back( tin );
}

void Heat::after_solve () {
    std::string name = this->get_name();
    std::string dirout = this->get_dirout();
    if ( stg.Tmax )
        write_double(dirout + "/" + name + "_Tmax", subsample(Tmax, stg.nmaxout));
    if ( stg.Tmin )
        write_double(dirout + "/" + name + "_Tmin", subsample(Tmin, stg.nmaxout));
    if ( stg.Ts )
        write_double(dirout + "/" + name + "_Ts", subsample(Ts, stg.nmaxout));
    if ( stg.qs )
        write_double(dirout + "/" + name + "_qs", subsample(qs, stg.nmaxout));
    if ( stg.t )
        write_double(dirout + "/" + name + "_t", subsample(t, stg.nmaxout));
    if ( stg.tsnap )
        write_double(dirout + "/" + name + "_tsnap", tsnap);
}
