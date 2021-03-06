import serial, socket

#from rwkos import amiLinux

def float_to_int(u_float):
    return (u_float+0.5).astype(int)


def Two_Char_Bytes_To_Int(msb, lsb):
    msb = ord(msb)
    lsb = ord(lsb)
    return 256*msb+lsb


def two_bytes(intin):
    intin = int(intin)
    msb = intin/256
    lsb = intin-msb*256
    return msb, lsb


def TwoIntBytesToInt(msb, lsb):
    return 256*msb+lsb

def TwoIntBytesToSignedInt(msb, lsb):
    output =  256*msb + lsb
    if output > (2**15 - 1):
        output = output - 2**16
    return output

def Negative_to_Twos(num):
    if num < 0:
        return 2**16+num
    else:
        return num


def Clean_Twos(int_in):
    if int_in > 2**15:
        return -(2**16-int_in)
    else:
        return int_in


def my_pause(n=100):
    i = 0
    for j in range(n):
        i+=1
        

def Open_Serial(portname=None, baud=115200, timeout = 5):
    """Open a serial connection on port portname with baud rate baud."""
    if portname is None:
        hostname = socket.gethostname()
        if hostname in ['ryan-CNC','hpdv4','ryan-duo-laptop']:
            portname = '/dev/ttyS0'
        else:
            portname = 'COM1'
    ser = serial.Serial(portname, baud, timeout=timeout)
    ## if amiLinux():
    ##     #portname = '/dev/ttyS0'
    ##     ser = serial.Serial(portname, baud, timeout=None)
    ## else:
    ##     ser = serial.Serial(portname, baud)
    ##     ser.
    return ser



def Close_Serial(ser):
    """Close the serial connection.  Only one connection to the port
    can be open at a time.  This can cause problems if a test doesn't
    finish cleanly and the port is left open.  If that happens, you
    will need to close IPython and reopen it."""
    ser.close()


def flush_ser(ser):
    ser.flushInput()
    ser.flushOutput()


def WriteInt(ser, intin):
    intout = Negative_to_Twos(intin)
    msb, lsb = two_bytes(intout)
    ser.write(chr(msb))
    ser.write(chr(lsb))


def WriteByte(ser, bytein):
    ser.write(chr(bytein))


def Read_Two_Bytes(ser):
    data1, data2 = ser.read(2)
    return Two_Char_Bytes_To_Int(data1, data2)


def Read_Byte(ser):
    data1 = ser.read(1)
    return ord(data1)


def Read_Line(ser):
    out = []
    i = 0
    while i < 1e5:
        data1 = ser.read(1)
        if data1 in ['\n','\r']:
            break
        out.append(data1)
        i += 1
    return out


def Read_Two_Bytes_Twos_Comp(ser):
    raw_int = Read_Two_Bytes(ser)
    return Clean_Twos(raw_int)


def Start_Test(ser):
    WriteByte(ser, 113)
    th0 = Read_Two_Bytes_Twos_Comp(ser)
    return th0


def Start_Test_Two_Motors(ser):
    WriteByte(ser, 113)
    th1 = Read_Two_Bytes_Twos_Comp(ser)
    th2 = Read_Two_Bytes_Twos_Comp(ser)
    return th1, th2
    
    
def Get_Theta(ser):
    WriteByte(ser, 114)
    th0 = Read_Two_Bytes_Twos_Comp(ser)
    return th0

def Get_Theta_Two_Motors(ser):
    WriteByte(ser, 114)
    th1 = Read_Two_Bytes_Twos_Comp(ser)
    th2 = Read_Two_Bytes_Twos_Comp(ser)
    return th1, th2

def Reset_Theta(ser):
    WriteByte(ser, 55)


def Stop_PSoC_ser(ser):
    WriteByte(ser, 115)


def Send_One_Voltage(ser, v=0):
    WriteByte(ser, 47)
    WriteInt(ser, v)
    

