#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as np
from scipy import signal as sci
from matplotlib import pyplot as plt

f = 1
T = 1/f
dt = T/20 #odstęp między próbkami dla 20 próbek
t = np.arange(0,T,dt) #generacja punktów w których pobrana zostanie próbka

sin = 20+20*np.sin(2*np.pi*f*t) #obliczenie wartości próbek przebiegu sinusoidalnego ze sładową stałą
sin = np.around(sin) #zaokrąglenie wartości próbek
sin = sin.astype(int)

tri = 20+20*sci.sawtooth(2*np.pi*f*t, 0.5) #obliczenie wartości próbek przebiegu trójkątnego ze składową stałą
tri = np.around(tri) #zaokrąglenie wartości próbek
tri = tri.astype(int)

np.set_printoptions(formatter={'int':hex})

print('sine:', sin)
print('triangle:', tri)

plt.plot(t, sin)
plt.plot(t, tri)

