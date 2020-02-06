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
    //thermal conductivity of medium
    for (i=0; i<n+1; i++) k.push_back( f_k(stg.k0, -ze[i]) );
    //specific heat
    for (i=0; i<n; i++) c.push_back( f_c(stg.c0, -zc[i]) );

    //thermal capacity
    cap.resize(n);
    //derivative of moisture w/r/t z
    dTdz.resize(n+1);
    //fluxes
    q.resize(n+1);

    //-----------------------------------
    //initial temperatures and capacities

    for (i=0; i<n; i++) {
        //temperature
        set_sol(i,
            f_geotherm(
                f_Ts(0, stg.Tsa, stg.Tsb, stg.Tsc),
                f_qgeo(stg.qgeo0, 0),
                f_k(stg.k0, 0),
                -zc[i]
            )
        );
        //thermal capacity
        cap[i] = f_cap(c[i], rho[i], get_sol(i));
    }

    //---------------
    //maximum stable time step

    double dt, tem;
    dtmax = INFINITY;
    for (long i=0; i<n+1; i++) {
        //get the appropriate (maximum) capacity
        if ( i == 0 ) {
            tem = c[0]*rho[0];
        } else if ( i == n ) {
            tem = c[n-1]*rho[n-1];
        } else {
            tem = c[i]*rho[i] > c[i-1]*rho[i-1] ? c[i]*rho[i] : c[i-1]*rho[i-1];
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

double Heat::f_cap (double c, double rho, double Tin) {
    //regular capacity
    double cap = c*rho;
    //apparent capacity, if needed
    if ( fabs(Tin - stg.Tf) <= stg.ahcw/2.0 )
        cap += stg.LH/stg.ahcw;
    return(cap);
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
        dTdt[i] = f_dTdt(q[i], q[i+1], f_cap(c[i], rho[i], Tin[i]), delz[i]);

}

double Heat::dt_adapt () {
    return(stg.dtfac*dtmax);
}

//------------------------------------------------------------------------------
//extras

void Heat::before_solve () {
	//initialize by taking a zero step
    this->step(0.0);
	//write static physical variables
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
