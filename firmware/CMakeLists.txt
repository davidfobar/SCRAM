cmake_minimum_required(VERSION 3.15.3)

project(firmware C CXX ASM)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(STM32CUBEMX_GENERATED_FILES
    Core/Src/main.cpp
    Core/Src/stm32wlxx_it.c
    Core/Src/stm32wlxx_hal_msp.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_adc.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_adc_ex.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_ll_adc.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_rcc.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_rcc_ex.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_flash.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_flash_ex.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_gpio.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_dma.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_dma_ex.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_pwr.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_pwr_ex.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_cortex.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_exti.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_i2c.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_i2c_ex.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_spi.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_spi_ex.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_tim.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_tim_ex.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_uart.c
    Drivers/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_uart_ex.c
    Core/Src/system_stm32wlxx.c

    Core/Inc/main.h
    Core/Inc/stm32wlxx_hal_conf.h
    Core/Inc/stm32wlxx_it.h

    Core/Src/stm32wlxx_hal_msp.c
    Core/Src/stm32wlxx_it.c
    Core/Src/system_stm32wlxx.c

    startup_stm32wle5xx.s
)

set(EXECUTABLE ${PROJECT_NAME}.out)

add_executable(${EXECUTABLE} ${STM32CUBEMX_GENERATED_FILES}
        source/application.cpp
        source/application.hpp
        source/digitalIO.cpp
        source/digitalIO.hpp
        )

target_compile_definitions(${EXECUTABLE} PRIVATE
        -DUSE_HAL_DRIVER
        -DSTM32WLE5xx
        )

target_include_directories(${EXECUTABLE} PRIVATE
        Core/Inc
        Drivers/STM32WLxx_HAL_Driver/Inc
        Drivers/CMSIS/Device/ST/STM32WLxx/Include
        Drivers/CMSIS/Include
        source
        )

target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -fno-exceptions
        -fdata-sections
        -ffunction-sections

        -Wall

        $<$<CONFIG:Debug>:-Og>
        )

target_link_options(${EXECUTABLE} PRIVATE
        -T${CMAKE_SOURCE_DIR}/STM32WLE5XX_FLASH.ld
        -mcpu=cortex-m4
        -mthumb
        -specs=nosys.specs
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
        )

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-size ${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)
