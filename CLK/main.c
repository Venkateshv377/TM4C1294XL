//---------------------------------------------------------------------------------
// Project: Blink TM4C TI-RTOS Using Clk (STARTER)
//
// Author: Eric Wilbur, www.MindshareAdvantage.com
//
// Date: Nov 2015
//
// Note: The function call TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT) HAS
//       to be in the ISR. This fxn clears the TIMER's interrupt flag coming
//       from the peripheral - it does NOT clear the CPU interrupt flag - that
//       is done by hardware. The author struggled figuring this part out - hence
//       the note. And, in the Swi lab, this fxn must be placed in the
//       Timer_ISR fxn because it will be the new ISR.
//
// Follow these steps to create this project in CCSv6.0:
// 1. Project -> New CCS Project
// 2. Select Template:
//    - TI-RTOS for Tiva-C -> Driver Examples -> EK-TM4C123 LP -> Example Projects ->
//      Empty Project
//    - Empty Project contains full instrumentation (UIA, RTOS Analyzer) and
//      paths set up for the TI-RTOS version of MSP430Ware
// 3. Delete the following files:
//    - Board.h, empty.c, EK_TM4C123GXL.c/h, empty_readme.txt
// 4. Add main.c from TI-RTOS Workshop Solution file for this lab
// 5. Edit empty.cfg as needed (to add/subtract) BIOS services, delete given Task
// 6. Build, load, run...
//----------------------------------------------------------------------------------


//----------------------------------------
// BIOS header files
//----------------------------------------
#include <xdc/std.h>  						//mandatory - have to include first, for BIOS types
#include <ti/sysbios/BIOS.h> 				//mandatory - if you call APIs like BIOS_start()
#include <xdc/runtime/Log.h>				//needed for any Log_info() call
#include <xdc/cfg/global.h> 				//header file for statically defined objects/handles
#include <xdc/runtime/Timestamp.h>			// used for Timestamp() calls


//------------------------------------------
// TivaWare Header Files
//------------------------------------------
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"

#include "driverlib/timer.h"


//----------------------------------------
// Prototypes
//----------------------------------------
void hardware_init(void);
void ledToggle(void);
void Timer_ISR(void);


//---------------------------------------
// Globals
//---------------------------------------
volatile int16_t i16ToggleCount = 0;
uint8_t ui8PinData = 1;

//---------------------------------------------------------------------------
// main()
//---------------------------------------------------------------------------
void main(void)
{
   hardware_init();							// init hardware via Xware
   BIOS_start();
}


//---------------------------------------------------------------------------
// hardware_init()
//
// inits GPIO pins for toggling the LED
//---------------------------------------------------------------------------
void hardware_init(void)
{
//	uint32_t ui32Period;

	SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0|GPIO_PIN_1);
	GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0|GPIO_PIN_1, 0x00);
}


//---------------------------------------------------------------------------
// ledToggle()
//
// toggles LED on Tiva-C LaunchPad
//---------------------------------------------------------------------------
void ledToggle(void)
{
	static uint32_t ui32_t0, ui32_t1, ui32_t2, ui32start, ui32stop, ui32delta;  // used for Timestamp calculations

	ui32_t0 = Timestamp_get32();									// calculate Timestamp() overhead (ui32_t2)
	ui32_t1 = Timestamp_get32();
	ui32_t2 = ui32_t1 - ui32_t0;

	GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, ui8PinData);   // toggle GPIO/LED
	if(ui8PinData==2)
	{
		ui32start = Timestamp_get32();								// get starting Timer snapshot for LED benchmark
		ui32stop = Timestamp_get32();								// get ending Timer snapshot for LED benchmark

		ui32delta = ui32stop - ui32start - ui32_t2;					// calculate LED toggle benchmark

		Log_info1("LED BENCHMARK = [%u] TM4C CYCLES", ui32delta);	// send LED benchmark to Log display
		ui8PinData=1;
	}
	else
	{
		ui8PinData=ui8PinData*2;
	}

	i16ToggleCount += 1;											// keep track of #toggles

	Log_info1("LED TOGGLED [%u] TIMES",i16ToggleCount);				// send toggle count to UIA

}
