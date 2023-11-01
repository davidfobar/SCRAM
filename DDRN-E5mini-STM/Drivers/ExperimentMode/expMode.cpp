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

struct ExpData {
	float SiPM_Temp;
	float SiPM_Signal;
} expData;

enum ExpCMDS {
	RETURN_TEMP = 0x01,
	RETURN_SAMPLE = 0x02,
	RETURN_HELLO = 0x03,
	SET_BIAS = 0x04
};

const int expDataLen = 8;

void packExpData(const ExpData *data, uint8_t *buf){
	int32_t SiPM_Temp = (int32_t)(data->SiPM_Temp * 100.0);
	int32_t SiPM_Signal = (int32_t)(data->SiPM_Signal * 100.0);

	buf[0] = (uint8_t)(SiPM_Temp >> 24);
	buf[1] = (uint8_t)(SiPM_Temp >> 16);
	buf[2] = (uint8_t)(SiPM_Temp >> 8);
	buf[3] = (uint8_t)(SiPM_Temp >> 0);

	buf[4] = (uint8_t)(SiPM_Signal >> 24);
	buf[5] = (uint8_t)(SiPM_Signal >> 16);
	buf[6] = (uint8_t)(SiPM_Signal >> 8);
	buf[7] = (uint8_t)(SiPM_Signal >> 0);
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
	uint8_t expDataBuf[8];
	uint8_t biasGain;

	while(1){
		// Wait for the command to be received
		if (HAL_UART_Receive(&huart1, rxCmdBuffer, 2, HAL_MAX_DELAY) == HAL_OK) {
			// Extract the command from the buffer
			command = rxCmdBuffer[0];

			switch(command) {
				case RETURN_TEMP:
					expData.SiPM_Temp = detector.getSiPMtemp(true);
					expData.SiPM_Signal = 0;
					packExpData(&expData, expDataBuf);
					HAL_UART_Transmit(&huart1, expDataBuf, expDataLen, 1000);
					break;
				case RETURN_SAMPLE:
					//read the dose first, otherwise the temperature will not be valid
					expData.SiPM_Signal = detector.readAccumulatedDose();
					expData.SiPM_Temp = detector.getSiPMtemp(false);

					packExpData(&expData, expDataBuf);
					HAL_UART_Transmit(&huart1, expDataBuf, expDataLen, 1000);
					break;
				case RETURN_HELLO:
					sprintf(msg, "hi\r\n");
					HAL_UART_Transmit(&huart1, (uint8_t *)&msg, strlen(msg), 1000);
					break;
				case SET_BIAS:
					biasGain = rxCmdBuffer[1];
					if (detector.setSiPM_Bias(biasGain)){
						sprintf(msg, "_OK\r\n");
					} else {
						sprintf(msg, "bad\r\n");
					}
					HAL_UART_Transmit(&huart1, (uint8_t *)&msg, strlen(msg), 1000);
					break;
				default:
					expData.SiPM_Temp = 0;
					expData.SiPM_Signal = 0;
					packExpData(&expData, expDataBuf);
					HAL_UART_Transmit(&huart1, expDataBuf, expDataLen, 1000);
					break;
			}
		}
	}
}
