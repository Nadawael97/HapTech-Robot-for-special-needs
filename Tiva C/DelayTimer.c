#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "C:/Keil/labware/inc/hw_ints.h"
#include "C:/Keil/labware/inc/hw_timer.h"
#include "C:/Keil/labware/inc/hw_types.h"
#include "C:/Keil/labware/inc/hw_memmap.h"
#include "C:/ti/TivaWare_C_Series-2.1.4.178/driverlib/rom.h"
#include "C:/ti/TivaWare_C_Series-2.1.4.178/driverlib/sysctl.h"
#include "C:/ti/TivaWare_C_Series-2.1.4.178/driverlib/timer.h"
#include "C:/ti/TivaWare_C_Series-2.1.4.178/driverlib/interrupt.h"
#include "C:/Keil/labware/inc/hw_nvic.h"
#include "C:/ti/TivaWare_C_Series-2.1.4.178/driverlib/pin_map.h"
#include "C:/ti/TivaWare_C_Series-2.1.4.178/driverlib/rom_map.h"

static unsigned long milliseconds = 0;

void timerInit(void)
{

		SysTickPeriodSet(0x00FFFFFF);
    SysTickEnable();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
    TimerConfigure(TIMER5_BASE, TIMER_CFG_PERIODIC_UP);

    TimerLoadSet(TIMER5_BASE, TIMER_A, SysCtlClockGet()/1000);

    IntEnable(INT_TIMER5A_TM4C123);
    TimerIntEnable(TIMER5_BASE, TIMER_TIMA_TIMEOUT);

    TimerEnable(TIMER5_BASE, TIMER_A);

    IntMasterEnable();

}

unsigned long micros(void)
{
	return (milliseconds * 1000) + (HWREG(TIMER5_BASE + TIMER_O_TAV) / 80);
}

unsigned long millis(void)
{
	return milliseconds;
}

void delayMicroseconds(unsigned int us)
{
	volatile unsigned long elapsedTime;
	unsigned long startTime = HWREG(NVIC_ST_CURRENT);
	do{
		elapsedTime = startTime-(HWREG(NVIC_ST_CURRENT) & 0x00FFFFFF);
	}
	while(elapsedTime <= us*80);
}

void delay(uint32_t milliseconds)
{
		unsigned long i;
		for(i=0; i<milliseconds; i++){
			delayMicroseconds(1000);
		}
}

void Timer5IntHandler(void)
{
    TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);

	milliseconds++;
}


