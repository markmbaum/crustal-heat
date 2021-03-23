# crustal-heat

One-dimensional heat transfer in a planet's crust

Used in:
* [Wordsworth, R., Knoll, A.H., Hurowitz, J. et al. *A coupled model of episodic warming, oxidation and geochemical transitions on early Mars*. Nature Geoscience. 14, 127–132 (2021).](https://doi.org/10.1038/s41561-021-00701-8)
* [Baum, M. & Wordsworth, R. *Groundwater Flow to Gale Crater in an Episodically Warm Climate*. JGR Planets 125, (2020).](https://www.doi.org/10.1029/2020JE006397)

--------

This repository provides a C++ class for solving the one-dimensional heat equation with mixed boundary conditions—prescribed temperature at the surface and geothermal gradient at the bottom—both of which may vary in time. Thermal properties (density, conductivity, specific heat) may also be spatially variable.

The class uses finite volumes and a time stepper from [libode](https://github.com/wordsworthgroup/libode). It automatically uses a time step near the largest stable value (based on thermal properties). Latent heat can be enabled to simulate freezing and thawing of ground ice/water. Openmp and the class-based structure of this code make it useful for running a large number of independent integrations with wide ranges of initial conditions and/or physical parameters.

See the [**documentation**](https://wordsworthgroup.github.io/crustal-heat/) for details with source code.
