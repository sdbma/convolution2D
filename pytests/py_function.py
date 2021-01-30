import numpy as np
import math
from scipy.signal import convolve2d
def my_conv(img, fltr):
    a = np.array(img)
    aw = int(math.sqrt(len(a)))
    a = a.reshape(aw,aw)
    b = np.array(fltr)
    bw = int(math.sqrt(len(b)))
    b = b.reshape(bw,bw)
    #print(type(a), a)
    #print(type(b), b)
    c = convolve2d(a, b, mode='same')
    #print(type(c), c)
    c = c.reshape(-1)
    return list(c)
