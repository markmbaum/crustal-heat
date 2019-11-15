from os import listdir
from os.path import join, basename
from numpy import *
import matplotlib.pyplot as plt

from reading import *

#-------------------------------------------------------------------------------
#INPUT

#directory with results in it
resdir = join('..', 'out')

#snapped variables to plot
snapvars = ['T', 'q']

#labels of snapped vars
snaplabs = ['$T$ (K)', '$q$ (W/m$^2$)']

#whether to plot grid variables
grid = True

#-------------------------------------------------------------------------------
#MAIN

#plot the grid spacing
if grid:
    fig, axs = plt.subplots(1,2)
    ze = readvar(resdir, 'ze')
    axs[0].plot(diff(ze), -ze[:-1])
    axs[0].invert_yaxis()
    axs[0].set_ylabel('Depth (m)')
    axs[0].set_xlabel('Cell Depth (m)')
    axs[1].plot(log10(diff(ze)), -ze[:-1])
    axs[1].invert_yaxis()
    axs[1].set_xlabel('log$_{10}$ Cell Depth (m)')
    fig.suptitle('Grid Spacing (%d cells)' % (len(ze)-1))

#plot results
assert len(snapvars) == len(snaplabs), 'different number of snap variables and labels'
n = len(snapvars)
fig, axs = plt.subplots(1,n)
ze = readvar(resdir, 'ze')
zc = readvar(resdir, 'zc')
S = {v:readsnaps(resdir, v) for v in snapvars}
L = dict(zip(snapvars, snaplabs))
for i,v in enumerate(S):
    s = S[v]
    if s.shape[0] == len(zc):
        z = zc
    elif s.shape[0] == len(ze):
        z = ze
    else:
        raise(ValueError)
    axs[i].plot(s, -z)
    axs[i].invert_yaxis()
    axs[i].set_xlabel(L[v])
axs[0].set_ylabel('Depth (m)')

plt.show()
