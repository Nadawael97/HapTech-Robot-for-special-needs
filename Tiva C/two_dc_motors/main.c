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


void SystemInit(){};


int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
	                   SYSCTL_XTAL_16MHZ);
	
	/*
	 * set the PWM clock to the system clock.
	 */
	 SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	 SysCtlDelay(10); //wait until it is ready

	 /*
	  * enabling PWM peripheral and port B
	  */

	 SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	 //SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	 SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);   // Enable the GPIOD peripheral for the output Direction Signal
	 SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);  //for the output PWM signal
	 SysCtlDelay(10); //wait until it is ready
	 /*
	  * port B configuration
	  */
	 GPIOPinConfigure(GPIO_PB6_M0PWM0);                        //Pin PB6 as PWM 
	 GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
	 GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);
	 GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0);       //Pin PB0 Direction for Motor(0-false or 5V-true)
	 GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2);       //Pin PB1 Direction for Motor(0-false or 5V-true)
	 /*
	  * port D configuration
	  */
		/*
	 GPIOPinConfigure(GPIO_PD0_M1PWM0);                        //Pin PD0 as PWM (M1PWM0)
	 GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
	 */
	 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);       //Pin PD3 Direction for Motor(0-false or 5V-true)
	 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);       //Pin PD2 Direction for Motor(0-false or 5V-true)

	 /*
	  * configure PWM0 
	  */
	 PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
	                     PWM_GEN_MODE_NO_SYNC);
	 PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
	                     PWM_GEN_MODE_NO_SYNC);
	 								 
	 SysCtlDelay(10);

	  /*Set the period. For a 50 KHz desired PWM frequency, the period = 1/50.000, or 20 microseconds. For a 50 MHz clock, this translates
     into 1000 clock ticks(20microsecs/0.02microsecs) .*/
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 100000);
    SysCtlDelay(10); 
		
		PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 100000);
		SysCtlDelay(10); 
	 
	 /*
	  * duty cycle
	  */
	 //Set the pulse width of PWM0 for a 25% duty cycle.
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 98000);
    SysCtlDelay(10); 

		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 98000);
    SysCtlDelay(10); 
		
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    SysCtlDelay(10);   
		PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    SysCtlDelay(10); 
		
		
		                        //Enable PWM signal at the given duty cycle(0% for now)
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);

		
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 5); 
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0); 
		
		PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 5); 
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);   
	  
	 while (1){
		
		
		 
		
	 }

  return 0;
	 
}
