from numpy import *
from pandas import read_csv
from os import listdir
from os.path import join
from scipy.interpolate import CubicSpline
import matplotlib.pyplot as plt

#-------------------------------------------------------------------------------
#INPUT

dirbatch = 'results'

fnout = 'thaw_times.csv'

#-------------------------------------------------------------------------------
#MAIN

#read the trials csv
trials = read_csv(join(dirbatch, fntrials), index_col=0)

#find thaw times
trials['t'] = nan
for idx in trials.index:
    t = fromfile(join(dirbatch, str(idx) + '_t'))
    minT = fromfile(join(dirbatch, str(idx) + '_minT'))
    s = CubicSpline(t, minT - 273)
    try:
        trials.at[idx,'t'] = s.roots()[0]
    except IndexError:
        print(idx)

#write to file
trials.to_csv(fnout)
