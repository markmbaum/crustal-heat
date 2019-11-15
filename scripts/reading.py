from numpy import *
from os import listdir
from os.path import join

__all__ = ['isint', 'readvar', 'readsnaps']

def isint(x):
    try:
        int(x)
    except (TypeError, ValueError):
        return(False)
    else:
        return(True)

readvar = lambda resdir, fn: fromfile(join(resdir, fn))

def readsnaps(resdir, varname):
    #get potential file names
    fns = listdir(resdir)
    #filter for the ones with the variable name
    fns = [fn for fn in fns if (('_' + varname + '_' in fn) and isint(fn[-1]))]
    #sort by the snap number
    fns = sorted(fns, key=lambda fn: int(fn.split('_')[-1]))
    #read the files
    x = [readvar(resdir, fn) for fn in fns]
    #put into a 2D array
    x = stack(x).T

    return(x)
