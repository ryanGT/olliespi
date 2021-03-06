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

responses = []
ilist = range(1,10)
all_responses = []

for i in ilist:
    n = i
    data = spi.xfer([n,0])
    n_echo = data[0]
    responses.append(n_echo)
    all_responses.append(data)
    
responses = concatenate((responses[1:],[responses[0]]), axis = 0)

t2 = time.time()
 
spi.close()

dt = t2-t1
print 'responses = ' + str(responses)
print 'dt = %0.4g' % dt	
N = len(ilist)
tpl = dt/N
print('time per loop = %0.4g' % tpl)


