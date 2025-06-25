# -*- coding: utf-8 -*-
"""
Created on Thu Apr 17 07:08:10 2025

@author: dimit
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

data = pd.read_csv("C:\\Users\\dimit\\Documents\\Undergraduate\\SPIN Work\\U235 sigF.csv", header = None)
E = data.iloc[:,0]
sig = data.iloc[:,1]

E = np.array(E)
sig = np.array(sig)

##vert group bdrs
g1x = np.array([1e-7,1e-7])
g1y = np.array([0,1e6])

g2x = np.array([3e-2,3e-2])
g2y = np.array([0,1e6])

#horiz lines

l1x = np.array([1e-11,1e-7])
l1y = np.array([6e3,6e3])
l2x = np.array([1e-7,3e-2])
l2y = np.array([10,10])
l3x = np.array([3e-2,3e1])
l3y = np.array([1.5,1.5])

fig  = plt.figure(dpi = 600)
plt.plot(E/1e6,sig)
plt.plot(g1x,g1y, 'r--')
plt.plot(g2x,g2y, 'r--')
plt.plot(l1x,l1y, 'r')
plt.plot(l2x,l2y, 'r')
plt.plot(l3x,l3y, 'r')
plt.ylim(2e-1,1e5)
plt.xscale("log")
plt.yscale("log")
plt.xlabel("Incident Neutron Energy (MeV)")
plt.ylabel("Fission Cross Section (barns)")

plt.grid()
plt.show()