################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/david.fobar/Documents/GitHub/SCRAM/LoRaWan-E5-Node/Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini.c \
C:/Users/david.fobar/Documents/GitHub/SCRAM/LoRaWan-E5-Node/Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini_radio.c 

OBJS += \
./Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini.o \
./Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini_radio.o 

C_DEPS += \
./Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini.d \
./Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini_radio.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini.o: C:/Users/david.fobar/Documents/GitHub/SCRAM/LoRaWan-E5-Node/Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini.c Drivers/BSP/STM32WLxx_LoRa_E5_mini/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DSTM32WLE5xx -DUSE_HAL_DRIVER -c -I../../Core/Inc -I../../LoRaWAN/App -I../../LoRaWAN/Target -I../../../../../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../../../../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../../../../../Utilities/trace/adv_trace -I../../../../../../Utilities/misc -I../../../../../../Utilities/sequencer -I../../../../../../Utilities/timer -I../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../Middlewares/Third_Party/LoRaWAN/LmHandler/Packages -I../../../../../../Middlewares/Third_Party/SubGHz_Phy -I../../../../../../Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver -I../../../../../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../../../../../Middlewares/Third_Party/LoRaWAN/Crypto -I../../../../../../Middlewares/Third_Party/LoRaWAN/Mac/Region -I../../../../../../Middlewares/Third_Party/LoRaWAN/Mac -I../../../../../../Middlewares/Third_Party/LoRaWAN/LmHandler -I../../../../../../Middlewares/Third_Party/LoRaWAN/Utilities -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/STM32WLxx_LoRa_E5_mini -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini_radio.o: C:/Users/david.fobar/Documents/GitHub/SCRAM/LoRaWan-E5-Node/Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini_radio.c Drivers/BSP/STM32WLxx_LoRa_E5_mini/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DSTM32WLE5xx -DUSE_HAL_DRIVER -c -I../../Core/Inc -I../../LoRaWAN/App -I../../LoRaWAN/Target -I../../../../../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../../../../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../../../../../Utilities/trace/adv_trace -I../../../../../../Utilities/misc -I../../../../../../Utilities/sequencer -I../../../../../../Utilities/timer -I../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../Middlewares/Third_Party/LoRaWAN/LmHandler/Packages -I../../../../../../Middlewares/Third_Party/SubGHz_Phy -I../../../../../../Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver -I../../../../../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../../../../../Middlewares/Third_Party/LoRaWAN/Crypto -I../../../../../../Middlewares/Third_Party/LoRaWAN/Mac/Region -I../../../../../../Middlewares/Third_Party/LoRaWAN/Mac -I../../../../../../Middlewares/Third_Party/LoRaWAN/LmHandler -I../../../../../../Middlewares/Third_Party/LoRaWAN/Utilities -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/STM32WLxx_LoRa_E5_mini -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-STM32WLxx_LoRa_E5_mini

clean-Drivers-2f-BSP-2f-STM32WLxx_LoRa_E5_mini:
	-$(RM) ./Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini.d ./Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini.o ./Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini.su ./Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini_radio.d ./Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini_radio.o ./Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini_radio.su

.PHONY: clean-Drivers-2f-BSP-2f-STM32WLxx_LoRa_E5_mini

