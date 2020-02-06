import sys
from numpy import *
from os import listdir
from os.path import join
from shutil import copyfile
from pandas import read_csv, DataFrame
from multiprocessing import Pool

try:
    import matplotlib.pyplot as plt
except ImportError:
    plotting = False
else:
    plotting = True

#-------------------------------------------------------------------------------
#INPUT

cpus = int(sys.argv[1])

#directory with binary files
resdir = 'results'

#directory to put formatted results into
fordir = 'formatted-results'

#name of trials table
fntri = 'trials.csv'

#-------------------------------------------------------------------------------
#FUNCTIONS

def format_trial(i):

    #read results for the trial
    zc = fromfile(join(resdir, str(i) + '_zc'))
    t = fromfile(join(resdir, str(i) + '_t'))
    Ts = fromfile(join(resdir, str(i) + '_Ts'))
    tsnap = fromfile(join(resdir, str(i) + '_tsnap'))
    fns = [fn for fn in listdir(resdir) if (str(i) + '_T_' == fn[:len(str(i) + '_T_')])]
    fns = sorted(fns, key=lambda fn: int(fn.split('_')[-1]))
    T = stack([fromfile(join(resdir,fn)) for fn in fns]).T
    #create and dump a DataFrame
    df = DataFrame(flipud(T), index=-zc[::-1], columns=tsnap).round(3)
    df.columns = ['temperature snapshot (K) at %.4g seconds' % float(col) for col in df.columns]
    df.to_csv(join(fordir, str(i) + '.csv'), index_label='depth (m)')
    #make a plot if possible
    if plotting:
        fig, (axa, axb) = plt.subplots(2,1)
        #surface temp plot
        axa.plot(t/(3600*24*365.25), Ts)
        axa.set_xlabel('Time (years)')
        axa.set_ylabel('Surface Temperature (K)')
        #temperature field plot
        r = axb.contourf(tsnap/(3600*24*365.25), zc, T)
        cb = plt.colorbar(r, ax=axb)
        cb.set_label('Temperature (K)')
        axb.set_xlabel('Time (years)')
        axb.set_ylabel('Z (m)')
        #format and save
        axa.set_title('Trial %d' % i)
        fig.tight_layout()
        fig.savefig(join(fordir, str(i)))

#-------------------------------------------------------------------------------
#MAIN

if __name__ == '__main__':

    #first, copy the trials csv
    copyfile(join(resdir, fntri), join(fordir, fntri))

    #now read the trials index
    tr = read_csv(join(fordir, fntri), index_col=0)

    #format in parllel
    pool = Pool(cpus)
    pool.map(format_trial, tr.index)
    pool.close()
