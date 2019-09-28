#include <stdbool.h>
#include <stdint.h>
#include "C:/Keil/labware/inc/tm4c123gh6pm.h"
#include "C:/Keil/labware/inc/hw_memmap.h"
#include "C:/Keil/labware/inc/hw_types.h"
#include "C:/Keil/labware/inc/hw_ints.h"
#include "C:/Keil/labware/inc/hw_gpio.h"
#include "C:/Keil/labware/inc/hw_sysctl.h"
#include "C:/ti/TivaWare_C_Series-2.1.2.111/driverlib/debug.h"
#include "C:/ti/TivaWare_C_Series-2.1.4.178/driverlib/gpio.h"
#include "C:/ti/TivaWare_C_Series-2.1.2.111/driverlib/pin_map.h"
#include "C:/ti/TivaWare_C_Series-2.1.2.111/driverlib/pwm.h"
#include "C:/ti/TivaWare_C_Series-2.1.2.111/driverlib/sysctl.h"
#include "C:/ti/TivaWare_C_Series-2.1.2.111/driverlib/uart.h"
#include "C:/ti/TivaWare_C_Series-2.1.2.111/utils/uartstdio.h"
#include "C:/ti/TivaWare_C_Series-2.1.2.111/driverlib/rom.h"
#include "C:/ti/TivaWare_C_Series-2.1.4.178/driverlib/pin_map.h"
#include "C:/ti/TivaWare_C_Series-2.1.4.178/driverlib/interrupt.h"
#include "C:/ti/TivaWare_C_Series-2.1.2.111/driverlib/sysctl.h"

/*static const uint32_t g_pui32GPIOBaseAddrs[] =
{
    GPIO_PORTA_BASE, GPIO_PORTA_AHB_BASE,
    GPIO_PORTB_BASE, GPIO_PORTB_AHB_BASE,
    GPIO_PORTC_BASE, GPIO_PORTC_AHB_BASE,
    GPIO_PORTD_BASE, GPIO_PORTD_AHB_BASE,
    GPIO_PORTE_BASE, GPIO_PORTE_AHB_BASE,
    GPIO_PORTF_BASE, GPIO_PORTF_AHB_BASE,
    GPIO_PORTG_BASE, GPIO_PORTG_AHB_BASE,
    GPIO_PORTH_BASE, GPIO_PORTH_AHB_BASE,
    GPIO_PORTJ_BASE, GPIO_PORTJ_AHB_BASE,
    GPIO_PORTK_BASE, GPIO_PORTK_BASE,
    GPIO_PORTL_BASE, GPIO_PORTL_BASE,
    GPIO_PORTM_BASE, GPIO_PORTM_BASE,
    GPIO_PORTN_BASE, GPIO_PORTN_BASE,
    GPIO_PORTP_BASE, GPIO_PORTP_BASE,
    GPIO_PORTQ_BASE, GPIO_PORTQ_BASE,
    GPIO_PORTR_BASE, GPIO_PORTR_BASE,
    GPIO_PORTS_BASE, GPIO_PORTS_BASE,
    GPIO_PORTT_BASE, GPIO_PORTT_BASE,
};
*/
void SystemInit(){};

void InitConsole (void){
	/*
	* Enable GPIO port A which is used for UART0 pins.
	*  change this to whichever GPIO port you are using.
	*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	/*
	 * Configure the pin muxing for UART0 functions on port A0 and A1.
	   This step is not necessary if your part does not support pin muxing.
	   TODO: change this to select the port/pin you are using.
	*/

	GPIOPinConfigure(GPIO_PA0_U0RX );
	GPIOPinConfigure(GPIO_PA1_U0TX);

	/*
	 * Enable UART0 so that we can configure the clock.
	 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	/*
	 * Use the internal 16MHz oscillator as the UART clock source.
	 */
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

	/*
	 * Select the alternate (UART) function for these pins.
     * change this to select the port/pin you are using.
	 */
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	/*
	 * initialize the UART for console I/O
	 */
	UARTStdioConfig(0, 115200, 16000000);
}


/*
 * main.c
 */
int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
	                   SYSCTL_XTAL_16MHZ);
	
	/*
	 * set the PWM clock to the system clock.
	 */
	 SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

	 InitConsole();

	 /*
	  * display the setup on the console.
	  */
	 UARTprintf("PWM ->\n");
	 UARTprintf("  Module: PWM0\n");
	 UARTprintf("  Module: PWM1\n");
	 UARTprintf("  Pin: PB6\n");
	 UARTprintf("  Pin: PB7\n");
	 UARTprintf("  Configured PB6 Duty Cycle: 25%%\n");
	 UARTprintf("  Inverted PB7 Cycle: 75%%\n");
	 UARTprintf("Generating PWM on PWM0 (PB6) -> State = ");
	 UARTprintf("Generating PWM on PWM1 (PB7) -> State = ");

	 /*
	  * enabling PWM peripheral and port B
	  */

	 SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	 SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	 SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	 /*
	  * port B configuration
	  */
	 GPIOPinConfigure(GPIO_PB6_M0PWM0);
	 GPIOPinConfigure(GPIO_PB7_M0PWM1);
	 GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
	 GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);

	 /*
	  * configure PWM0 and PWM1 to count up/down with synchronous
	  */
	 PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
	                     PWM_GEN_MODE_SYNC);

	 PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
             PWM_GEN_MODE_SYNC);

	 /*
	  * frequency setting
	  */
	 PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 64000);
	 PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 64000);

	 /*
	  * duty cycle
	  */
	 PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,
	                      PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0) / 4);

	 PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1,
	                      PWMGenPeriodGet(PWM1_BASE, PWM_GEN_0) / 4);

	 while (1){

		 /*
		  * enable the PWM0 PB6 and PWM1 PB7
		  */
		 PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, false);
		 PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
		 PWMGenEnable(PWM0_BASE, PWM_GEN_0);

		 SysCtlDelay((SysCtlClockGet() * 5) / 3);

		 PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, false);
		 PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
		 PWMGenEnable(PWM1_BASE, PWM_GEN_0);

		 SysCtlDelay((SysCtlClockGet() * 5) / 3);
	 }

  return 0;

}
