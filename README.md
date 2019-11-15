# crustal-heat
Numerics for the one-dimensional heat equation applied to a planet's crust

This repository provides a class for solving the one-dimensional heat equation with mixed boundary conditions, a temperature at the surface and a geothermal gradient at the bottom. It allows spatially variable thermal properties and time-varying boundary conditions. The class uses finite volumes and a time stepper from [libode](https://github.com/wordsworthgroup/libode). It also chooses its own time step to maximize efficiency. This code is useful for efficiently solving the heat equation over wide ranges of physical parameters which require many independent integrations.
