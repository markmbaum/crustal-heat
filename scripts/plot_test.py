from numpy import *
from os import listdir
from os.path import join
from scipy.special import erf
import matplotlib.pyplot as plt

from reading import *

#-------------------------------------------------------------------------------
#INPUT

#directory with results in it
testdir = join('..', 'out')

#-------------------------------------------------------------------------------
#FUNCTIONS

f_ex = lambda x: 1 + erf(x/sqrt(4*0.02))

#-------------------------------------------------------------------------------
#MAIN

#temperature output files
fnT = [fn for fn in listdir(testdir) if '_T_' in fn]
#number of trials
n = max(int(i.split('_')[0]) for i in fnT) + 1
#list of final temperature arrays
Ts = [readvar(testdir, str(i) + '_T_1') for i in range(n)]
#list of cell coordinate arrays
zcs = [readvar(testdir, str(i) + '_zc') for i in range(n)]
#grid spacings
hs = readvar(testdir, 'delz')

#plot values at bottom
fig, ax = plt.subplots(1,1)
ax.hist([T[0] for T in Ts])
ax.set_xlabel('Domain Bottom Value at $t_f$')
ax.set_ylabel('Counts')

#plot convergence
fig, ax = plt.subplots(1,1)
err = array([abs(T - f_ex(zc)).max() for T,zc in zip(Ts,zcs)])
ax.loglog(hs, err, 'k.', label='results')
for i in range(1,4):
    hh = logspace(log10(hs.min()), log10(hs.max()), 1000)
    b = log10(err[0]/(10**(i*log10(hs[0]))))
    y = 10**(i*log10(hh) + b)
    mask = (y > min(err))
    hh = hh[mask]
    y = y[mask]
    ax.loglog(hh, y, ':', label='$\propto (\Delta t)^{%d}$' % i)
ax.legend()
ax.set_xlabel('cell width')
ax.set_ylabel('max abs error')
ax.set_title('convergence test')

plt.show()
