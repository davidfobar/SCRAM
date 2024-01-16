################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Drivers/BSP/Environment_Sensors/env_sensors.cpp 

OBJS += \
./Drivers/BSP/Environment_Sensors/env_sensors.o 

CPP_DEPS += \
./Drivers/BSP/Environment_Sensors/env_sensors.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Environment_Sensors/%.o Drivers/BSP/Environment_Sensors/%.su: ../Drivers/BSP/Environment_Sensors/%.cpp Drivers/BSP/Environment_Sensors/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WLE5xx -c -I../Core/Inc -I../Drivers/ExperimentMode -I../Drivers/BSP/Environment_Sensors/BMP390 -I../Drivers/BSP/Environment_Sensors/LSM303 -I../Drivers/BSP/Environment_Sensors -I../Drivers/BSP/Detector -I"C:/Users/david.fobar/Documents/GitHub/SCRAM/DDRN-E5mini-STM/Drivers/BSP/STM32WLxx_Nucleo" -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/CMSIS/Include -I../LoRaWAN/App -I../LoRaWAN/Target -I../Utilities/trace/adv_trace -I../Utilities/misc -I../Utilities/sequencer -I../Utilities/timer -I../Utilities/lpm/tiny_lpm -I../Middlewares/Third_Party/LoRaWAN/LmHandler/Packages -I../Middlewares/Third_Party/SubGHz_Phy -I../Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../Middlewares/Third_Party/LoRaWAN/Mac/Region -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/LmHandler -I../Middlewares/Third_Party/LoRaWAN/Utilities -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Environment_Sensors

clean-Drivers-2f-BSP-2f-Environment_Sensors:
	-$(RM) ./Drivers/BSP/Environment_Sensors/env_sensors.d ./Drivers/BSP/Environment_Sensors/env_sensors.o ./Drivers/BSP/Environment_Sensors/env_sensors.su

.PHONY: clean-Drivers-2f-BSP-2f-Environment_Sensors

