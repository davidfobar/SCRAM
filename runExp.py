#create new screen session
# screen -S exp

#open old screen session
# screen -r exp


import numpy as np
from serial import Serial
from datetime import datetime
import struct
import time

from CZT import CZT
from XRS4_driver import XRS4

xray_serialport = '/dev/ttyUSB0'
det_serialport = '/dev/ttyACM0'

RETURN_TEMP   = 0x01
RETURN_SAMPLE = 0x02
RETURN_HELLO  = 0x03
SET_BIAS      = 0x04
RETURN_BATT   = 0x05
TOGGLE_RED    = 0x06

def getFloatValue(ser, cmd):
    assert(cmd == RETURN_TEMP or cmd == RETURN_SAMPLE or cmd == RETURN_BATT)
    ser.write(bytes([cmd, 0x00]))
    data = ser.read(4)
    return struct.unpack('>i', data)[0]/100

def testConnection(ser):
    ser.write(bytes([RETURN_HELLO, 0x00]))
    data = ser.read(8)
    return data.strip().decode('utf-8') 

def setBias(ser, bias):
    assert(bias >= 0 and bias <= 255)
    ser.write(bytes([SET_BIAS, bias]))
    data = ser.read(8)
    return data.strip().decode('utf-8') 

tempChamber = CZT()

timestamp = datetime.now()
"""
Min_temp = -10
Max_temp = 21
Step_Temp = 10
Min_dose = 1
Max_dose = 11
Step_dose = 5
Min_bias = 5
Max_bias = 11
Step_bias = 5
"""

Temp_Range = [-10, -5, 0, 5, 10, 15, 20, 25, 30, 35 ]
Dose_Range = [1, 5, 10, 15]
Bias_Range = [5, 10, 15, 20, 25, 50]
temp_record = []


data = np.zeros( (len(Temp_Range), len(Dose_Range), len(Bias_Range)) )

with Serial(xray_serialport, 57600, timeout=1) as xray_ser:
    xray = XRS4(xray_ser)
    with Serial(det_serialport, 115200, timeout=2) as det_ser:
        for i, setTemp in enumerate(Temp_Range):
            print(f'set temp {setTemp}')
            curTemp = getFloatValue(det_ser, RETURN_TEMP)
            diff = setTemp*0.05
            while curTemp > setTemp+diff or curTemp < setTemp-diff:
                curTemp = getFloatValue(det_ser, RETURN_TEMP)
                time.sleep(5)
                print("  Current temperature is:", curTemp, datetime.now())
            #setting the temperature
            #if temp <= 5:
                #print("temp is less than 5")
                #tempChamber.setTemperatureSP(temp-10)
                #wait for temp at SiPM
                #while temp < getFloatValue(det_ser, RETURN_TEMP):
                   # time.sleep(5) #wait 5 seconds
                #tempChamber.setTemperatureSP(getFloatValue(det_ser,RETURN_TEMP))
            #elif temp > 5 and temp < 20:
                #print("temp is between 5 and 20")
                #tempChamber.setTemperatureSP(temp)
                #while temp != getFloatValue(det_ser, RETURN_TEMP):
                    #time.sleep(5) #wait 5 seconds
                #tempChamber.setTemperatureSP(getFloatValue(det_ser,RETURN_TEMP))
            #elif temp >= 20: 
                #print("temp is greater than 20")
                #tempChamber.setTemperatureSP(temp+5)
                #while temp != getFloatValue(det_ser, RETURN_TEMP):
                    #time.sleep(5) #wait 5 seconds
                #tempChamber.setTemperatureSP(getFloatValue(det_ser,RETURN_TEMP))

            for j, dose in enumerate(Dose_Range):
                print(f'   set dose {dose}')
                xray.set_pulse_count(dose)
                for k, bias in enumerate(Bias_Range):
                    #measure
                    xray.fire_xray()
                    setBias(det_ser, bias)
                    data[i,j,k] = getFloatValue(det_ser, RETURN_SAMPLE)
                    temp_record.append((setTemp, dose, bias, getFloatValue(det_ser, RETURN_TEMP)))
                    setBias(det_ser, 0)
                    print(f'    measure bias {bias}')
                

with open(f'data_{timestamp}') as outfile:
    np.save(outfile, data)

with open(f'temp_record_{timestamp}') as outfile: 
    np.save(outfile, np.array(temp_record))

print(f' file saved to {outfile}.npy')