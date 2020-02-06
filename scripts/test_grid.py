from numpy import *
import matplotlib.pyplot as plt

#-------------------------------------------------------------------------------
# INPUT

depth = 2.5
delz0 = 0.02
delzfrac = 1.01
delzmax = 0.08

#-------------------------------------------------------------------------------
# MAIN

ze = [0, delz0]
while ze[-1] < depth:
    delz = (ze[-1] - ze[-2])*delzfrac
    if delz > delzmax:
        delz = delzmax
    ze.append( ze[-1] + delz )

ze = array(ze)
ze *= -depth/ze.max()
ze = ze[::-1]

fig, axs = plt.subplots(1,2)
axs[0].plot(diff(ze), -ze[:-1])
axs[0].invert_yaxis()
axs[0].set_ylabel('Depth (m)')
axs[0].set_xlabel('Cell Depth (m)')
axs[1].plot(log10(diff(ze)), -ze[:-1])
axs[1].invert_yaxis()
axs[1].set_xlabel('log$_{10}$ Cell Depth (m)')
fig.suptitle('Grid Spacing (%d cells)' % (len(ze)-1))

plt.show()
