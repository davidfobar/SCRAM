/*
 * expMode.cpp
 *
 *  Created on: Nov 1, 2023
 *      Author: dave
 */

#include "expMode.h"
#include "sys_app.h"
#include "string.h"
#include <stdio.h>

enum ExpCMDS {
	RETURN_TEMP   = 0x01,
	RETURN_SAMPLE = 0x02,
	RETURN_HELLO  = 0x03,
	SET_BIAS      = 0x04,
	RETURN_BATT   = 0x05
};

#define EXP_MODE_ACTIVE true

void uartSendFloatVal(float val_f){
	int32_t val = (int32_t)(val_f * 100.0);

	uint8_t buf[4];
	buf[0] = (uint8_t)(val >> 24);
	buf[1] = (uint8_t)(val >> 16);
	buf[2] = (uint8_t)(val >> 8);
	buf[3] = (uint8_t)(val >> 0);
	HAL_UART_Transmit(&huart1, buf, 4, 1000);
}

void enterExperimentMode(void){
	disableLoRaWAN();
	MX_USART1_UART_Init();

	char msg[8];
	sprintf(msg, "hello\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t *)&msg, strlen(msg), 1000);

	uint8_t command;

	// Buffer to store the received command
	uint8_t rxCmdBuffer[2];

	while(1){
		// Wait for the command to be received
		if (HAL_UART_Receive(&huart1, rxCmdBuffer, 2, HAL_MAX_DELAY) == HAL_OK) {
			// Extract the command from the buffer
			command = rxCmdBuffer[0];

			switch(command) {
				case RETURN_TEMP:
					uartSendFloatVal( detector.sampleSiPMtemp() );
					break;

				case RETURN_SAMPLE:
					uartSendFloatVal( detector.sampleSiPMsignal() );
					break;

				case RETURN_HELLO:
					sprintf(msg, "hi\r\n");
					HAL_UART_Transmit(&huart1, (uint8_t *)&msg, strlen(msg), 1000);
					break;

				case SET_BIAS:
					if (detector.setSiPM_Bias( rxCmdBuffer[1] )){
						sprintf(msg, "OK\r\n");
					} else {
						sprintf(msg, "BAD\r\n");
					}
					HAL_UART_Transmit(&huart1, (uint8_t *)&msg, strlen(msg), 1000);
					break;

				case RETURN_BATT:
					uartSendFloatVal( bsp_env_sensors.getBattVoltage() );
					break;

				default:
					sprintf(msg, "BAD\r\n");
					HAL_UART_Transmit(&huart1, (uint8_t *)&msg, strlen(msg), 1000);
					break;
			}
		}
	}
}
