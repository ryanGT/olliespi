# Ollie Langhorst
#Robotics Research under Dr. Krauss
# 11/24/2015

from matplotlib.pyplot import *
from numpy import *

import time
import spidev

spi = spidev.SpiDev()
spi.open(0,0)

spi.max_speed_hz = 4000000

t1 = time.time()

ilist = range(1,10)
#ilist = [1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0]
N = len(ilist)
#responses = zeros(N, int)


#responses = spi.xfer(ilist,4000000,10)
responses = []

for i in ilist:
    n = i
    data = spi.xfer([n,0])
    #n_echo = data[0]
    #responses[i-1] = n_echo
    responses.append(data)

#responses = concatenate((responses[1:],[responses[0]]), axis = 0)

t2 = time.time()
 
spi.close()

dt = t2-t1
print 'responses = ' + str(responses)
print 'dt = %0.4g' % dt	
N = len(ilist)
tpl = dt/N
print('time per loop = %0.4g' % tpl)


