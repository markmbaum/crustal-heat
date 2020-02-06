#ifndef UTIL_H_
#define UTIL_H_

//! \file util.h

#include <cmath>
#include <vector>
#include <cstdio>
#include <cstdlib>

//!finds max of array
double max (double *a, long n);

//!finds min of array
double min (double *a, long n);

//!interpolates a 1d array
double interp (double *x, double *y, double xx, long n);

//!creates an evenly spaced vector of values over a range
std::vector<double> linspace (double a, double b, long n);

//!create an logarithmically spaced vector of values over a range
std::vector<double> logspace (double a, double b, long n);

//!subsamples a vector, keeping first and last elements
/*!
\param[in] v vector to subsample
\param[in] n approximate length target
*/
std::vector<double> subsample (std::vector<double> v, unsigned long n);

#endif
