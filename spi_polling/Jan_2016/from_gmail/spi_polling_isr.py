"""
Ollie Langhorst
Robotics Research for Dr. Krauss
1/5/2016
Before this script is run, it is necessary to start the pigpio daemon.
Do this by: sudo pigpiod
"""

print('sup, 3/7/16 10:47PM')

from matplotlib.pyplot import *
from scipy import *

import time
import pigpio
import serial_utils

pi1 = pigpio.pi()

channel = 0
bps = 1000000 
mode = 3
h = pi1.spi_open(channel, bps, mode)

#~ n = 100
n = 5

t1 = time.time()
responses = []
ilist = range(1,n)
testbyte = 0
sendindex_list = []
allbytes = []
ms = 1.0/1000
sleep_time = ms
echo_responses = []
all_bytes = []
ab = all_bytes.append

for i in ilist:
    (b,testbyte) = pi1.spi_read(h,1)
    ab(testbyte)
    poll_count = 0
    while (testbyte[0] != 1):
        poll_count += 1
        #time.sleep(sleep_time)#asking for 1/10th of millisecond
        (b,testbyte) = pi1.spi_read(h,1)
        ab(testbyte)
        if poll_count > 100:
            print('poll_count exceeded')
            break
    imsb, ilsb = serial_utils.two_bytes(i) 
    #pi1.spi_write(h, ilsb)
    (b, one_check) = pi1.spi_read(h,1)
    (b, nlsb) = pi1.spi_read(h,1)
    ab(nlsb)
    (b, nmsb) = pi1.spi_read(h,1)
    ab(nmsb)
    ## (b, vlsb) = pi1.spi_read(h,1)
    ## ab(vlsb)
    ## (b, vmsb) = pi1.spi_read(h,1)
    ## ab(vmsb)    
    #~ cursend = pi1.spi_read(h,1)
    #~ (count, rx_data) = pi1.spi_xfer(h,[imsb, ilsb, 5, 5, 5])
    #~ print imsb, ilsb
    #~ nlsb = rx_data[1]
    #~ nmsb = rx_data[2]
    #~ vlsb = rx_data[3]
    #~ vmsb = rx_data[4]
    #~ currow = [testbyte, nlsb, nmsb, cursend, vlsb, vmsb, poll_count]
    currow = [testbyte[0], nlsb[0], nmsb[0]]#, vlsb[0], vmsb[0], poll_count]
    allbytes.append(currow)
    #sendindex_list.append(cursend)
    n_int = serial_utils.TwoIntBytesToInt(nmsb[0], nlsb[0])
    n_echo = serial_utils.Clean_Twos(n_int)
    ## echo_int = serial_utils.TwoIntBytesToSignedInt(vmsb[0],vlsb[0])
    ## echo_ans = serial_utils.Clean_Twos(echo_int)
    responses.append(n_echo)
    ## echo_responses.append(echo_ans)
    
t2 = time.time()

pi1.stop()

dt = t2-t1
print 'responses = ' + str(responses)
print 'dt = %0.4g' % dt	
N = len(ilist)
tpl = dt/N
print('time per loop = %0.4g' % tpl)

fd = 1.0/tpl
print('digital update frequency = %0.4g Hz' % fd)

test_r = array(responses)-responses[1]+1
test_diff = test_r-arange(n-1)

print('='*30)
print('test_diff = ')
print(str(test_diff))

## figure(1)
## clf()
## ilist
## plot(ilist, echo_responses,'ro')

show()
