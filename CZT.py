from pyModbusTCP.client import ModbusClient
from time import sleep

c = ModbusClient(host="169.254.112.221", port=502, auto_open=True)
ip = 
#this is a list of relative addresses for the registers
PROCESS = 0x0F
TEMP_SP = 0x24 #temperature setpoint
TEMP = 0x23 #temperature
HUM_SP = 0x29 #humidity setpoint
HUM = 0x28 #humidity

class CZT(ip = "169.254.112.221", p=502):
    def __init__(self, ip):
        self.c = ModbusClient(host=ip, port=p, auto_open=True)

    def setTemperatureSP(self, temperature):
        #write as an unsigned integer, and multiply by 10 to store the decimal place
        temperature = int(temperature*10)
        if temperature < 0:
            #use two's complement to represent negative numbers
            temperature = 0xFFFF + temperature + 1
        return self.c.write_single_register(TEMP_SP, int(temperature))

    def getTemperatureSP(self):
        temperature = self.c.read_holding_registers(TEMP_SP, 1)[0]
        if temperature > 0x7FFF:
            #use two's complement to represent negative numbers
            temperature = 0xFFFF - temperature + 1
            temperature = -temperature
        return temperature/10

    def getTemperature(self):
        temperature = self.c.read_holding_registers(TEMP, 1)[0]
        if temperature > 0x7FFF:
            #use two's complement to represent negative numbers
            temperature = 0xFFFF - temperature + 1
            temperature = -temperature
        return temperature/10

    def setHumiditySP(self, humidity):
        #ensure humidity is between 0 and 100
        if humidity < 0:
            humidity = 0
        elif humidity > 100:
            humidity = 100
        humidity = int(humidity*10)

        return self.c.write_single_register(HUM_SP, int(humidity))

    def getHumiditySP(self):
        humidity = self.c.read_holding_registers(HUM_SP, 1)[0]
        return humidity/10

    def getHumidity(self):
        humidity = self.c.read_holding_registers(HUM, 1)[0]
        return humidity/10
  