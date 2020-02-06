from numpy import *
from os.path import join
from pandas import read_csv
import matplotlib.pyplot as plt

#-------------------------------------------------------------------------------
#INPUT

fntrials = 'thaw_times.csv'

yrsec = 3600*24*365.35

Tsa = 220
Tsbs = [285, 295, 305]
qgeo = 0.04

#-------------------------------------------------------------------------------
#MAIN

#read results
df = read_csv(fntrials, index_col=0)

#convert thaw times to years
df['t'] /= yrsec

#------------------
#simple plot

fig, ax = plt.subplots(1,1)
Tsa = df['Tsa'].values[argmin(abs(df['Tsa'].values - Tsa))]
qgeo = df['qgeo0'].values[argmin(abs(df['qgeo0'].values - qgeo))]
cmap = plt.get_cmap('Set1')
colors = [cmap(i) for i in (6,4,0)]
for i,Tsb in enumerate(Tsbs):
    Tsb_ = df['Tsb'].values[argmin(abs(df['Tsb'].values - Tsb))]
    sl = df[(df['Tsa'] == Tsa) & (df['qgeo0'] == qgeo) & (df['Tsb'] == Tsb_)]
    label = '$T_{sf} = %g$ K' % Tsb
    ax.semilogy(sl['k0'], sl['t'], '.:', color=colors[i], label=label)
ax.legend()
ax.set_xlabel(r'Thermal Conductivity $\alpha_r$ (W/m$\cdot$K)')
ax.set_ylabel('Time to Fully Thawed Crust (Earth years)')
ax.set_ylim(1e4, 1e6)

#------------------
#4 dimensional plot

#convert thaw times to log years
df['t'] = log10(df['t'])

#filter
df = df[df['k0'].isin([1, 3, 5]) & df['qgeo0'].isin([0.03, 0.05, 0.07])]
df = df[(df['Tsa'] <= 240) & (df['Tsa'] >= 210)]
df = df[(df['Tsb'] <= 310)]
print(10**df['t'].min(), 10**df['t'].max())

#create and format subplots
k0u = sorted(df['k0'].unique())
qgeo0u = sorted(df['qgeo0'].unique())
nk0 = len(k0u)
nqgeo0 = len(qgeo0u)
fig, axs = plt.subplots(nk0, nqgeo0)
#fig.subplots_adjust(wspace=0.01, hspace=0.01)
for i in range(nk0):
    for j in range(1,nqgeo0):
        axs[i,j].set_yticklabels([])
        axs[i,j].tick_params(axis='y', length=0)
for i in range(nk0-1):
    for j in range(nqgeo0):
        axs[i,j].set_xticklabels([])
        axs[i,j].tick_params(axis='x', length=0)
for i in range(nk0):
    axs[i,-1].text(1.05, 0.5, '$k = %g$' % k0u[i],
            ha='left', va='center', rotation=270,
            transform=axs[i,-1].transAxes)
for j in range(nqgeo0):
    axs[0,j].text(0.5, 1.05, '$q_{geo} = %g$' % qgeo0u[j],
            ha='center', va='bottom', transform=axs[0,j].transAxes)
fig.text(0.4, 0.01, 'Initial Surface Temperature (K)', ha='center', va='bottom')
fig.text(0.01, 0.5, 'Final Surface Temperature (K)', ha='left', va='center', rotation=90)

#make plots
k02i = dict(zip(k0u, range(nk0)))
qgeo02j = dict(zip(qgeo0u, range(nqgeo0)))
Tsau = sorted(df['Tsa'].unique())
Tsbu = sorted(df['Tsb'].unique())
g = df.groupby(['k0', 'qgeo0']).groups
vmin = floor(df['t'].min())
vmax = ceil(df['t'].max())
levels = arange(vmin, vmax+0.25, 0.25)
for idx in g:
    ax = axs[k02i[idx[0]], qgeo02j[idx[1]]]
    sl = df.loc[g[idx]].sort_values(['Tsb', 'Tsa'])
    t = sl['t'].values.reshape(len(Tsbu), len(Tsau))
    r = ax.contourf(Tsau, Tsbu, t, vmin=vmin, vmax=vmax, levels=levels, cmap='plasma_r')

cb = fig.colorbar(r, ax=axs)
cb.set_label('Years to Full Thaw', rotation=270, va='bottom')
cb.set_ticks( sorted(list(set(int(i) for i in levels))) )
cb.set_ticklabels( [('$10^{%g}$' % i) for i in cb.get_ticks()] )
plt.show()
