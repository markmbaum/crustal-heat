//! \file util.cc

#include "util.h"

double max (double *a, long n) {
    double r = 0.0;
    long i;
    //first find a minimum or let m stay at zero
    for (i=0; i<n; i++) if (a[i] < r) r = a[i];
    //now that m is less than or equal to the lowest element of a, find the max
    for (i=0; i<n; i++) if (a[i] > r) r = a[i];
    return(r);
}

double min (double *a, long n) {
    double r = 0.0;
    long i;
    //first find a maximum or let m stay at zero
    for (i=0; i<n; i++) if (a[i] > r) r = a[i];
    //now that m is greater than or equal to the largest element of a, find the min
    for (i=0; i<n; i++) if (a[i] < r) r = a[i];
    return(r);
}

double interp (double *x, double *y, double xx, long n) {

    if ( xx <= x[0] )
        return(y[0]);
    for (long i=0; i<n-1; i++)
        if ( (xx >= x[i]) && (xx <= x[i+1]) )
            return( y[i] + (xx - x[i])*(y[i+1] - y[i])/(x[i+1] - x[i]) );
    return(y[n-1]);
}

std::vector<double> linspace (double a, double b, long n) {

    //avoid division by zero
    if ( n == 1 ) {
        if ( a != b ) {
            printf("FAILURE: cannot linspace with a single value if the range limits are not identical");
            exit(EXIT_FAILURE);
        }
        std::vector<double> v(1, a);
        return(v);
    }
    //spacing
    double h = (b - a)/(n - 1);
    //values
    std::vector<double> v;
    for (long i=0; i<n; i++)
        v.push_back( a + i*h );

    return(v);
}

std::vector<double> subsample (std::vector<double> v, unsigned long n) {

    //calculate the approximate interval size
    unsigned long size = v.size();
    unsigned long step = size/n;
    //fill a new vector
    if ( (step == 0) || (step == 1) ) {
        //just copy the vector
        std::vector<double> r(v.begin(), v.end());
        //finish
        return(r);
    } else {
        //step is not zero, subsample the vector
        std::vector<double> r;
        r.push_back( v[0] );
        unsigned long i;
        for (i=step; i<size; i+=step) r.push_back(v[i]);
        if (i < size - 1) r.push_back( v.back() );
        //finish
        return(r);
    }
}
