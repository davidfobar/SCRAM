import time
'''
Command, Description, Return, Value(s)
#PLSCNT|xxx, Set the pulse count of the unit (200 or less pulses) !PLSCNT|xxx xxx = pulse count (<200)
#DLY|xxx, Set the delay time of the unit (15-240 seconds) !DLY|xxx xxx = delay (sec)
#TRSET|xxx|yyy, Set the number of pulse trains and the time between pulse trains !TRSET|xxx|yyy, xxx = pulse trains, yyy = time between (<240 sec)
#DUTYWARN|x, Turn the duty cycle warning on the X-Ray on or off, !DUTYWARN|ON or !DUTYWARN|OFF, x = 1 to turn warning on, x = 0 to turn warning off
#START_NO, Fire the X-Ray with no delay time, !MSG|Firing X-Ray
#START_DEL, Fire the X-Ray with the delay (will continue to fire all trains if greater than 1), !MSG|Firing X-Ray
#STOP, Stop the X-Ray pulsing, !MSG|X-Ray Stopped and !PULSE|xxx, xxx = number of pulses counted by the unit (!PULSE message will return every time the X-Ray stops firing)
#PPC|x, Set the number of pulses per count for the XR-150, !PPC|x, x = number of pulses per count (1 or 3)
?PID, Returns the serial number and head code of the unit, !PID|xxxxx|yyyyyyy|zzz, xxxxx = serial number, yyyyyyy = head code, zzz = unit (150-12, 150-20, 200, 3, 4)
?VER, Returns the program version, !VER|x.yy, x = main version number, yy = sub version number
?BAT, Returns the current battery level as a percentage, !BAT|xx, xx = battery level as percentage
?PLSCNT, Returns the current set pulse count, !PLSCNT|xxx, xxx = pulse count
?DLY, Returns the set delay time, !DLY|xxx, xxx = delay (sec)
?TRCNT, Returns the set number of trains, !TRCNT|xxx, xxx = pulse trains
?TRBTW, Returns the time between pulse trains !TRBTW|xxx, xxx = time between trains (sec)
?LIFE, Returns the life count of the unit, !LIFE|xxxxxx, xxxxxx = life count of unit (pulses)
?STATUS, Returns whether the X-Ray is pulsing or not, !STATUS|x, x = 0 when not pulsing/counting down, x = 1 when pulsing, x = 2 when counting down
?DUTYWARN, Returns if the duty cycle warning is on or off, !DUTYWARN|ON or !DUTYWARN|OFF
?CYCT, Returns the number of pulses before hitting the duty cycle warning, !CYCT|xxx, xxx = number of pulses before duty cycle
?EXCEED, Returns duty cycle and voided warranty numbers, !EXCEED|xxx|yyy|zzz, xxx = # of times the duty cycle has been reached, yyy = highest number of pulses over the duty cycle, zzz = number of times x-ray fired with settings that may void the warranty
?PPC, Returns the current set pulses per count on the XR-150, !PPC|x, x = number of pulses per count (1 or 3)
-, Errors, !ERR|x - [Message], x = error number, Message = short description of error

The following is a list of error messages that may be returned by the unit:
Error Number, Description
0, Invalid command - input command not recognized
1, Firing not started - unit did not start firing when sent the command. Make sure it is not already firing, the pulse count is not 0, or the unit is waiting for the duty cycle cooldown
2, Input settings are greater than the duty cycle (200 pulses/4 min). User will need to change the settings (pulses, # of trains, time between trains) to keep X-Ray under 200/4min. For the XR-150: The duty cycle is 300 pulses/4min or 100 counts/4min
3, Delay time out of range - must be between 15 - 240 seconds
4, Set pulse count first - user must set the pulse count before setting the number of trains
5, Time between our of range - must be between 1 - 240 seconds
6, Number of pulse trains needs to be greater than 0
7, Low battery - battery needs to be charged
8, Duty cycle - the unit has reached the duty cycle, wait a bit for cooldown
9, No feedback - no feedback was detected when pulsing X-Ray
10, No pulse detected - a pulse wasnt detected within one second of the previos one
11, Failsafe - warning LED circuit is detecting a bad warning LED, check board
12, Pulses per count input not valid - XR-150 only!
'''

#The following is a class that uses the above commands to control the XRS4 X-Ray through a serial connection
#This class abstracts the serial connection and the commands to make it easier to use the XRS4
#The experimentalist should only need to use the following functions:
# - set_pulse_count(int count)
# - set_delay(int seconds)
# the class will manage the number of trains and time between trains to keep the duty cycle below 200 pulses/4min
# - fire_xray()
# - stop_xray()
# - get_battery_level()
# - get_pulse_count()
# - get_delay()
# - get_life_count()
# - get_status()
# - get_duty_cycle_warning()
# - get_cycles_before_warning()
# - get_pulses_per_count()
# - get_serial_number()
# - get_version()
# - get_head_code()
# - get_unit()
# - get_error()
class XRS4:
    def __init__(self, xray_serial):
        self.ser = xray_serial

        #get the serial number and head code of the unit
        self.ser.write(b'?PID\n')
        print(self.ser.readline())

        #get the program version
        self.ser.write(b'?VER\n')
        print(self.ser.readline())


    def fire_no_delay(self):
        #get the program version
        self.ser.write(b'?START_NO\r\n')
        print(self.ser.readline())
        self.ser.flush()

    def set_pulse_count(self, count):
        if count > 200:
            print("Error: Pulse count must be less than 200")
            return
        self.ser.write(b'#PLSCNT|' + str(count).encode() + b'\n')
        print(self.ser.readline())

    def set_delay(self, seconds):
        if seconds < 15 or seconds > 240:
            print("Error: Delay must be between 15 and 240 seconds")
            return
        self.ser.write(b'#DLY|' + str(seconds).encode() + b'\n')

    def fire_xray(self):
        self.ser.write(b'#START_NO\n')
        print(self.ser.readline())