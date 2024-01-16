################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/LmhpCompliance.c \
../Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/LmhpPackagesRegistration.c 

C_DEPS += \
./Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/LmhpCompliance.d \
./Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/LmhpPackagesRegistration.d 

OBJS += \
./Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/LmhpCompliance.o \
./Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/LmhpPackagesRegistration.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/%.o Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/%.su: ../Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/%.c Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WLE5xx -c -I../Core/Inc -I../Drivers/ExperimentMode -I../Drivers/BSP/Environment_Sensors/LSM303 -I../Drivers/BSP/Detector -I../Drivers/BSP/Environment_Sensors/BMP390 -I"C:/Users/david.fobar/Documents/GitHub/SCRAM/DDRN-E5mini-STM/Drivers/BSP/STM32WLxx_Nucleo" -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/CMSIS/Include -I../LoRaWAN/App -I../LoRaWAN/Target -I../Utilities/trace/adv_trace -I../Utilities/misc -I../Utilities/sequencer -I../Utilities/timer -I../Utilities/lpm/tiny_lpm -I../Middlewares/Third_Party/LoRaWAN/LmHandler/Packages -I../Middlewares/Third_Party/SubGHz_Phy -I../Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver -I../Middlewares/Third_Party/LoRaWAN/Crypto -I../Middlewares/Third_Party/LoRaWAN/Mac/Region -I../Middlewares/Third_Party/LoRaWAN/Mac -I../Middlewares/Third_Party/LoRaWAN/LmHandler -I../Middlewares/Third_Party/LoRaWAN/Utilities -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-LoRaWAN-2f-LmHandler-2f-Packages

clean-Middlewares-2f-Third_Party-2f-LoRaWAN-2f-LmHandler-2f-Packages:
	-$(RM) ./Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/LmhpCompliance.d ./Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/LmhpCompliance.o ./Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/LmhpCompliance.su ./Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/LmhpPackagesRegistration.d ./Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/LmhpPackagesRegistration.o ./Middlewares/Third_Party/LoRaWAN/LmHandler/Packages/LmhpPackagesRegistration.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-LoRaWAN-2f-LmHandler-2f-Packages

