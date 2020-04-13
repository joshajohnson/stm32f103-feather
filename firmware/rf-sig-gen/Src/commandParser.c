#include "main.h"
#include "commandParser.h"
#include "usbd_cdc_if.h"
#include "dwt_stm32_delay.h"
#include <stdio.h>
#include <string.h>
#include "max2871.h"

char txStr[128] = "";

// extern struct MAX2871Struct max2871Status;

void commandParser(struct MAX2871Struct *max2871Status)
{
	#define BUF_SIZE 128
	#define CMD_SIZE 32
	#define NUM_ARGS 6

	char usrMsg[BUF_SIZE];
	char command[CMD_SIZE] = "";
	char args[NUM_ARGS][16];
	uint8_t argNum = 0;
	// Get whatever the user has sent
	scanUSB((uint8_t *) usrMsg, BUF_SIZE);

	// Split on delim
	for (uint8_t i = 0; i < FIFO_SIZE; i++)
	{
		if ((usrMsg[i] == '(') || (usrMsg[i] == ')') || (usrMsg[i] == ','))
			usrMsg[i] = '\0';
	}

	// Determine the command word
	uint8_t i = 0;
	while (usrMsg[i] != '\0')
	{
		command[i] = usrMsg[i];
		i++;
	}

	// Find arguments
	while (i < FIFO_SIZE)
	{
		// If null term, (was delim) skip
		if (usrMsg[i] == '\0')
			i++;
		else
		{
			// Otherwise, copy arguments into their own strings
			uint8_t j = 0;
			while (usrMsg[i] != '\0')
			{
				args[argNum][j] = '\0';
				args[argNum][j++] = usrMsg[i];
				i++;
			}
			args[argNum][j] = '\0';
			argNum++;
		}

	}

	// Find command based on command word, and call function

	if (strncmp("on", command, 2) == 0)
	{
		HAL_GPIO_WritePin(nLED_USR_GPIO_Port, nLED_USR_Pin, 1);
	}

	else if (strncmp("off", command, 3) == 0)
	{
		HAL_GPIO_WritePin(nLED_USR_GPIO_Port, nLED_USR_Pin, 0);
	}

	else if (strncmp("init", command, 4) == 0)
	{
		max2871Setup(max2871Status);
	}

	else if (strncmp("status", command, 6) == 0)
	{
		if (strncmp("verbose", args[0], 7) == 0)
		{
			args[0][0] = (int32_t) "";
			max2871PrintStatus(VERBOSE,max2871Status);
		}
		else
			max2871PrintStatus(nVERBOSE,max2871Status);

		// txChainPrintStatus(txStatus);

		// readAD8302vRef(receiverStatus);
		// sprintf((char *)txStr, "> AD8302 VREF = %0.3f, VDELTA = %0.3f\n", receiverStatus->refVoltage, receiverStatus->refDelta);
		// printUSB(txStr);

	}

	else if (strncmp("WHOAMI", command, 5) == 0)
	{
		printUSB("> Josh's Signal Generator!\r\n");
	}

	else if ((strncmp("ls", command,2) == 0) || (strncmp("help", command, 4) == 0))
	{
		printUSB((char *)"> --  Available Commands  --\r\n");
		printUSB((char *)"> sigGen(frequency, power)\r\n");
		printUSB((char *)"> sweep(startFreq, stopFreq, numSteps, power, time)\r\n");
		printUSB((char *)"> status(verbose)\r\n");
		printUSB((char *)"> WHOAMI\r\n");
		printUSB((char *)"> Use the below at your own risk:\r\n");
		printUSB((char *)"> setMaxPower(power(dBm))\r\n");
		printUSB((char *)"> setAttenuation(atten(dB))\r\n");
		printUSB((char *)"> enableRF\r\n");
		printUSB((char *)"> disableRF\r\n");
		printUSB((char *)"> enablePA\r\n");
		printUSB((char *)"> disablePA\r\n");
	}
	else
	{
		printUSB("> Not found, try again\r\n");
		sprintf((char *)txStr, "> %s\n", command);
		printUSB(txStr);

	}

}
