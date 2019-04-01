/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include <string.h>

#include "Timer.h"
#include "BlinkLed.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal_uart.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 led blink sample (trace via DEBUG).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// Then demonstrates how to blink a led with 1 Hz, using a
// continuous loop and SysTick delays.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
#define BLINK_ON_TICKS  (TIMER_FREQUENCY_HZ * 3 / 4)
#define BLINK_OFF_TICKS (TIMER_FREQUENCY_HZ - BLINK_ON_TICKS)

// ----- Private variables ----------------------------------------------------
UART_HandleTypeDef huart5;

/* Private function prototypes -----------------------------------------------*/
static void UART5_Init(void);
static void GPIO_Init(void);

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char* argv[])

{
  // Initialize all of the peripherals
  UART5_Init();
  GPIO_Init();

  timer_start();

  blink_led_init();
  uint8_t receive_buffer[100]; // you have to hard code/ know the expected size of the incoming message
  uint8_t message[100][100];
  uint8_t Message[100][100] = {"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNNmdhhyyyyyyyyyyhdmNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNNdhyo/:://++oooo+:/+//::/:+hydmNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNmds/:::::-::://+ss+++++/+oooso+o//::oydNNMMMMMMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMNNdyo/--..-::+yyyyoosy++/::----::sss///o+syyhhmNMMMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMNmddh+o::+/-/+/:-+hddhy+::::-:..yyhyyhho+ys/--+syyhmNNMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMNhoshhooo/+oo++ossshhds::-/ssyyy/:smho++/+/o+:-/++ohdhyymNMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMNhssddhsoososoo+:/yyhyyyo+/+oyydmyshhmmyo++sso:.-:ohyhmdhhhmmNMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMNh/+hddhoosssys++++oyyhyyys++ossssoydmdmmmddddddho/:oyhmmmdmmdhdNNMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMms+ohhhys/os+oo+///+oyyyyyysooshssshmmmmmmmmmmmmmmmddddydmmmmNmhhdmNMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMNdysssoo:+o/os+++//++oyyyyhhyyshdhhddmmmmmddmmmmmmmmmmmmmdymmmmmNmmmNmmNMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMNhhhs/oo+shssyy++o+/oydddddhhhhos+odmdmddmmmdmmmmmmmmmmmmmmmNNNmmNNNNNNdmNMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMNhhhdh+++sdhodyys+osoymdddhysoydyssshdmdhhdmmmmmmmmmmmmmmmmmmmmNmmmmmdmNddmNMMMMMMMMMM",
		  "MMMMMMMMMMMMMNhhyhhyosyho/:/+yyosyyddddhyyyhdhhhyyhddddmmmdmmmmmmmmmmmmmmmmmmmNmdmmyhhsyhmNMMMMMMMMM",
		  "MMMMMMMMMMMMNdhss+/:/y/::--:+/ssyyshdddhyohyyddmddhhddhddmddmmdmmdmdmmmmmmmmNNmNdNmdmh/oydmNMMMMMMMM",
		  "MMMMMMMMMMMNdhyshhhhddo--../y+++yysosyys/yd+oddmmddddddddmhhdddmmddmmmmmmmmmmmNNmmmNNNyyshdmNMMMMMMM",
		  "MMMMMMMMMMMmhdoyyyhddhh/-:+o/ossyyhddhssssoo:sdmmmmmmmmddddhsmmmmydmmmmmmmmmmNNNmmNNNmddhhmmNMMMMMMM",
		  "MMMMMMMMMMNmhh/hmmdssyyysyho/dhsdddddyyyyyso/sdmddmdddhddmddhdmmmmmmmddmmmmmmNNNNNNNNmdNNmmNNNMMMMMM",
		  "MMMMMMMMMMmmmdddmdh+--ohhooosho:hdddyyh++oso/+oshhhyyyysyyhdhhhhmmmddmmmmmmmmmmNNNNmmNmNNmdmNNMMMMMM",
		  "MMMMMMMMMMdmmmdhddds/./yhyhs++:-yyhyyssddysysyoyoosyyyossoosyshhhdmmmmmmmmmmmmNmmdmNNNNmyhhymNMMMMMM",
		  "MMMMMMMMMNdmhodhdhhhyoohdhydy:-:/-+osdsohhyy+.::../ysoo+oooyhhdddhhhhhdmmmmmmNNNNNNmNNmdhsdhmNNMMMMM",
		  "MMMMMMMMMNyoooddmdmmmmdddsydys/--:/+sso+soy/+:/:.-/+/oooosyyhddyyhhddddhdmmmmmNNNNNNNNNmNNNmNNNMMMMM",
		  "MMMMMMMMMNyshmdddmmdmmmhddmmdhyyyyhmdyhddds:oo+///os+oo+ossyyyyyhhhhddhy+ydmmmNNNNNNNNNNNNmmNNNMMMMM",
		  "MMMMMMMMMNhhdhdhhdmmmmmmhhdmdhdmdddmmmhdddddmdy//:/:://:ossssyyyhhhhhhhhyyyhhdmmmNNNNNNNNNhdNNNMMMMM",
		  "MMMMMMMMMNymmmmmmmmddddmdddhdhhhdddydhyhddmmddh+..--.//ossssssysyyyhhhhyyyhyhhyhmmNNNNNNNNNNNNNMMMMM",
		  "MMMMMMMMMNyhmmmmmmddmmdddmmmdhdhddddyso/ssyyhhs+:-/+/+/oossyyyyyyyyhhhyyossyhyyhyhmNNmNNNNNNNNNMMMMM",
		  "MMMMMMMMMMdddddddmdmdhddmdmmmdddddyyhy++y+::-:ydo++so+ssyyyysssyyyyyyhysoo+yysyyydmmmmmmmmNNNNMMMMMM",
		  "MMMMMMMMMMNhdhhdhhhdhdddddddddhhhhddhhhhhhhhy/+yyo/oosysssssssooyyysyysosoossyhdmmmdmmNNmhmNNNMMMMMM",
		  "MMMMMMMMMMNdmdhdhhhydddmdddddddddhhdddddhhhhhyso//+++++ooso+osooosyoysso+sssooomNNNNNNNNmmNNNMMMMMMM",
		  "MMMMMMMMMMMNdmmmmdmdddmddddddddddmmddddmmddhhys+::/+++o+oyhysssosssosssssyyys-.+hhmNNNNNNNNNNMMMMMMM",
		  "MMMMMMMMMMMMdsmmmmmdmmddddhhhddmmddddddhddddhddhy+::/+++o+syhsossssoossssyy+-.//:++odmNNNNNNMMMMMMMM",
		  "MMMMMMMMMMMMNhosyddmddddddddhdddmmdmdhs+hmmdddhyyso+/:/:o/oshysssssssssosys-/ohdddyoyhmNNNNNMMMMMMMM",
		  "MMMMMMMMMMMMMNho:-/oshhddddddddmmddddddhsyhmddyo+osso+/:+oshysyyyyhysyssyyhhmNNNNNmhdmmNNNNMMMMMMMMM",
		  "MMMMMMMMMMMMMMNdho/.--+yhmddddmddmmmmmdho+hmmmmdddddhhh/:oyhyyyyyhhhhhhdmNNNNNNNNNNNNNNNNNMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMNmddy:..-:ydmmdmmdddmdmmdddmmmmmmdddmmms+oyyysyys:-::/sdmNNNNNNNNNNNNNNNNNMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMmhhdy/-../hmmmmmmddddhdddddmmddmmmmmms/yyyyyyyys/-.-+odmmmmmNNNNNNNNNNMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMNdyhdy/-.-+dmmmdddyyhddhhhdddddddmmdo/ysyhhhhdmmy+-:sshyhmmNNNNNNNNNMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMmysyddy+-:/yhmdd-yyssoyhhhdddddmd+:sysyyhyhhsyhh/+++hddddmNNNNNNMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMNmhyhdmyo/-:ssooy/++/+++ossyhhhy/o++sdddyoo--:o+o/shhhydNNNNNNMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMNmhsyhdys..+s//:.-./+//:/++/+//ssydNmhhhsosoyyyyyhhhmNNNNNMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMNdyyss/--:+++o+-+:`-:++/-.-+:+hdhhyyyyyhhhdyyhmmNNNNMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMNdyo++/---.-:-::+so/+os+//+//+++osssssyyhhdmNNMMMMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNmhy+/::::/syso//+++//--:--:::::/sydmmNNMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNNmhyo++o//::-..---::://oshdmNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNNNmmdddddddmmNNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
		  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"};

  // Infinite loop
  uint8_t flag = 1;
  int incomingPacketCounter = 0;
  while (flag)
  {
	//	  HAL_StatusTypeDef status = HAL_UART_Receive_IT(&huart5, receive_buffer, sizeof(receive_buffer));
	HAL_StatusTypeDef status = HAL_UART_Receive(&huart5,receive_buffer, sizeof(receive_buffer), HAL_MAX_DELAY);
	//	  HAL_StatusTypeDef status = HAL_UART_Receive(&huart5, message[incomingPacketCounter], sizeof(message[incomingPacketCounter]), HAL_MAX_DELAY);
	//	  trace_printf("receive %#08x\n", status);
	trace_printf("%s\n",receive_buffer);
	if(status == 0)
	{
	  strcpy(message[incomingPacketCounter],(const uint8_t*)receive_buffer);
	}
	//	  trace_printf("moop: %s\n",message[incomingPacketCounter]);
	incomingPacketCounter++;

	if(incomingPacketCounter == 52)
	{
	//		  trace_printf("PRINTING THE MEMORY \n");
	//		  trace_printf("%s/n", message[3]);
	//		  for(int i=0; i<10; i++)
	//		  {
	//			  trace_printf("%s",message[i]);
	//			  trace_printf("\n");
	//		  }
		flag = 0;
	}
	  HAL_Delay(100);
  }

  trace_printf("Download from camera complete \n");
  for(int i=0; i<52; i++)
  {

	  HAL_StatusTypeDef status = HAL_UART_Transmit(&huart5, Message[i],sizeof(Message[i]),HAL_MAX_DELAY);
	  trace_printf("Transmitting status %#08x\n", status);
	  HAL_Delay(1000);
  }

  trace_printf("Completed transmission to transceiver \n");
  // Infinite loop, never return.
}

void
UART5_Init(void)
{
    huart5.Instance = UART5;
    huart5.Init.BaudRate = 115200;
    huart5.Init.WordLength = UART_WORDLENGTH_8B;
    huart5.Init.StopBits = UART_STOPBITS_1;
    huart5.Init.Parity = UART_PARITY_NONE;
    huart5.Init.Mode = UART_MODE_TX_RX;
    huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart5.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart5) != HAL_OK)
    {
            trace_printf("UART 5 Error");
    }
}

void
GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
}

//void CAN_Init(void)
//{
//	 __HAL_RCC_CAN1_CLK_ENABLE();
//	 __GPIOx_CLK_ENABLE();
//}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
