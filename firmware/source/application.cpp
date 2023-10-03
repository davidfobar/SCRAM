#include "application.hpp"

#include "stm32wlxx_ll_gpio.h"
#include "main.h"

extern "C" {

void setup() {
    // For future episodes ;)
}

void loop() {
    LL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

    HAL_Delay(1000U);
}

}
