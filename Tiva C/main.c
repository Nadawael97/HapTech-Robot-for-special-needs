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
#include "C:/ti/TivaWare_C_Series-2.1.4.178/utils/uartstdio.h"
#include "C:/ti/TivaWare_C_Series-2.1.2.111/driverlib/rom.h"
#include "C:/ti/TivaWare_C_Series-2.1.4.178/driverlib/pin_map.h"
#include "C:/ti/TivaWare_C_Series-2.1.4.178/driverlib/interrupt.h"
#include "C:/ti/TivaWare_C_Series-2.1.2.111/driverlib/sysctl.h"
#include "C:/ti/TivaWare_C_Series-2.1.4.178/driverlib/timer.h"
#include "C:\ti\TivaWare_C_Series-2.1.4.178\third_party\FreeRTOS\Source\include\FreeRTOS.h"
#include "C:/ti/TivaWare_C_Series-2.1.2.111/driverlib/debug.h"
#include "C:/Keil/labware/inc/hw_nvic.h"
#include "C:/ti/TivaWare_C_Series-2.1.2.111/driverlib/fpu.h"

#define Period  320000 //(16000000/50) 50Hz
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portTICK_RATE_MS portTICK_PERIOD_MS

#define DC_motors  1

#define DC_motor1  0 //PB6
#define DC_motor2  0 //PB4
#define DC_motor3  0 //PB5
#define DC_motor4  0 //PE5


#define ServoMotors 1
/*
//gripper servo motor is 180 degrees 
//PC5
*/
#define SERVO_GRIPPER 1
#define SERVO_GRIPPER_STEPS         180     // Maximum amount of steps in degrees
#define SERVO_GRIPPER_MIN           8000 // The minimum duty cycle for this servo
#define SERVO_GRIPPER_MAX						36000

/*
//elbow servo motor is 180 degrees 
//PD6
*/
#define SERVO_ELBOW 1
#define SERVO_ELBOW_STEPS         180 
#define SERVO_ELBOW_MIN          8000    
#define SERVO_ELBOW_MAX						36000 

/*
//base servo motor is 360 degrees 
//PC6
*/
#define SERVO_BASE 1
#define SERVO_BASE_STEPS         360     // Maximum amount of steps in degrees (180 is common)
#define SERVO_BASE_MIN           11200     // The minimum duty cycle for this servo
#define SERVO_BASE_MAX           36800    // The maximum duty cycle

	
unsigned int base_servo_lut[SERVO_BASE_STEPS+1];
unsigned int elbow_servo_lut[SERVO_ELBOW_STEPS+1];
unsigned int gripper_servo_lut[SERVO_GRIPPER_STEPS+1];

#define Bluetooth_module


#define IR_sensor
#define IR_PORT          GPIO_PORTE_BASE
#define IR_PIN           GPIO_PIN_2
#define irmode           GPIO_PIN_2

#define Flame_sensor
#define FlameSensor_PORT          GPIO_PORTE_BASE
#define FlameSensor_PIN           GPIO_PIN_1
#define Flamemode                 GPIO_PIN_1


void 
SystemInit(){};

void 
IR_init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeGPIOInput(IR_PORT, IR_PIN);
	
}
	
void 
Flame_init(void)
{
	GPIOPinTypeGPIOInput(FlameSensor_PORT, FlameSensor_PIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
}

void 
InitServoMotors(void) 
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	//Gripper Servo
	GPIOPinConfigure(GPIO_PC5_WT0CCP1);
	GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_5);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
	TimerConfigure(WTIMER0_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_B_PWM);
	TimerLoadSet(WTIMER0_BASE, TIMER_B, (Period-1));
	TimerMatchSet(WTIMER0_BASE, TIMER_B, (Period-9600));
	TimerMatchSet(WTIMER0_BASE, TIMER_B, 1);
	TimerEnable(WTIMER0_BASE, TIMER_B);
	
	//base servo
	GPIOPinConfigure(GPIO_PC6_WT1CCP0);
	GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_6);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);
	TimerConfigure(WTIMER1_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM);
	TimerLoadSet(WTIMER1_BASE, TIMER_A, (Period-1));
	TimerMatchSet(WTIMER1_BASE, TIMER_A, (Period-9600));
	TimerMatchSet(WTIMER1_BASE, TIMER_A, 1);
	TimerEnable(WTIMER1_BASE, TIMER_A);
	
	//elbow servo
	GPIOPinConfigure(GPIO_PD6_WT5CCP0);
	GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_6);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER5);
	TimerConfigure(WTIMER5_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM);
	TimerLoadSet(WTIMER5_BASE, TIMER_A, (Period-1));
	TimerMatchSet(WTIMER5_BASE, TIMER_A, (Period-9600));
	TimerMatchSet(WTIMER5_BASE, TIMER_A, 1);
	TimerEnable(WTIMER5_BASE, TIMER_A);
	
}

//gripper servo
void
ConfigureGripperServo(void)
{
    unsigned int servo_stepval, servo_stepnow;
    unsigned int i;

    servo_stepval   = ( (SERVO_GRIPPER_MAX - SERVO_GRIPPER_MIN) / SERVO_GRIPPER_STEPS );
    servo_stepnow   = SERVO_GRIPPER_MIN;

    for (i = 0; i < (SERVO_GRIPPER_STEPS+1); i++)
    {
    	gripper_servo_lut[i] = (Period-servo_stepnow);
    	servo_stepnow += servo_stepval;
    }
}

void
SetGripperServoPosition(uint32_t position)
{
	TimerMatchSet(WTIMER0_BASE, TIMER_B, position);
}

void
SetGripperServoAngle(uint32_t angle)
{
	SetGripperServoPosition(gripper_servo_lut[angle]);
}

//elbow servo
void
ConfigureElbowServo(void)
{
    unsigned int servo_stepval, servo_stepnow;
    unsigned int i;

    servo_stepval   = ( (SERVO_ELBOW_MAX - SERVO_ELBOW_MIN) / SERVO_ELBOW_STEPS );
    servo_stepnow   = SERVO_ELBOW_MIN;

    for (i = 0; i < (SERVO_ELBOW_STEPS+1); i++)
    {
    	elbow_servo_lut[i] = (Period-servo_stepnow);
    	servo_stepnow += servo_stepval;
    }
}

void
SetElbowServoPosition(uint32_t position)
{
	TimerMatchSet(WTIMER5_BASE, TIMER_A, position);
}

void
SetElbowServoAngle(uint32_t angle)
{
	SetElbowServoPosition(elbow_servo_lut[angle]);
}

//base servo

void
ConfigureBaseServo(void)
{
    unsigned int servo_stepval, servo_stepnow;
    unsigned int i;

    servo_stepval   = ( (SERVO_BASE_MAX - SERVO_BASE_MIN) / SERVO_BASE_STEPS );
    servo_stepnow   = SERVO_BASE_MIN;

    for (i = 0; i < (SERVO_BASE_STEPS+1); i++)
    {
    	base_servo_lut[i] = (Period-servo_stepnow);
    	servo_stepnow += servo_stepval;
    }
}

void
SetBaseServoPosition(uint32_t position)
{
	TimerMatchSet(WTIMER1_BASE, TIMER_A, position);
}

void
SetBaseServoAngle(uint32_t angle)
{
	SetBaseServoPosition(base_servo_lut[angle]);
}

//DC motors
void
InitDCMotors(){
		
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	
	//control pins for right dc motors
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0);      
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_1);  
	
	//control pins for left dc motors
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_7);      
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0);
	
	//DC Motor left front
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	GPIOPinConfigure(GPIO_PB6_M0PWM0);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
									PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 100000);
	
	//DC Motor right front
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	GPIOPinConfigure(GPIO_PB4_M0PWM2);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
	PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN |
									PWM_GEN_MODE_NO_SYNC);	
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, 100000);
	
	//DC Motor right back
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	GPIOPinConfigure(GPIO_PE5_M0PWM5);
	GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_5);
	PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN |
									PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 100000);
	
	//DC Motor left back
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	GPIOPinConfigure(GPIO_PF0_M1PWM4);
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_0);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN |
									PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 100000);
}

void 
PWMDCMotorLeftFront(uint16_t dutyCycle){
	if (dutyCycle < 0 ){
		
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 0);
	
	}
	else
	{
		
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, dutyCycle);
	
	}
	
	PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
}

void
PWMDCMotorRightFront(uint16_t dutyCycle){
	
	if (dutyCycle < 0 ){
		
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 0);
	
	}
	else
	{
		
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, dutyCycle);
	
	}
	PWMGenEnable(PWM0_BASE, PWM_GEN_1);
	PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);
}

void
PWMDCMotorRightBack(uint16_t dutyCycle){
	
	if (dutyCycle < 0 ){
		
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, 0);
	
	}
	else
	{
		
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, dutyCycle);
	
	}
	PWMGenEnable(PWM0_BASE, PWM_GEN_2);
	PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
}

void
PWMDCMotorLeftBack(uint16_t dutyCycle){
	
	if (dutyCycle < 0 ){
		
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_4, 0);
	
	}
	else
	{
		
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_4, dutyCycle);
	
	}
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	PWMOutputState(PWM1_BASE, PWM_OUT_4_BIT, true);
}

void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
     SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
     GPIOPinConfigure(GPIO_PA0_U0RX);
     GPIOPinConfigure(GPIO_PA1_U0TX);
     GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

/*volatile int g_ui32Flags;


void Timer0IntHandler(void)
{
char cOne, cTwo;
TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
HWREGBITW(&g_ui32Flags, 0) ^= 1;
GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, g_ui32Flags << 1);
IntMasterDisable();
cOne = HWREGBITW(&g_ui32Flags, 0) ? '1' : '0';
cTwo = HWREGBITW(&g_ui32Flags, 1) ? '1' : '0';
UARTprintf("\rT1: %c T2: %c", cOne, cTwo);
IntMasterEnable();
}
void Timer1IntHandler(void)
{
char cOne, cTwo;
TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
HWREGBITW(&g_ui32Flags, 1) ^= 1;
GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, g_ui32Flags << 1);
IntMasterDisable();
cOne = HWREGBITW(&g_ui32Flags, 0) ? '1' : '0';
cTwo = HWREGBITW(&g_ui32Flags, 1) ? '1' : '0';
UARTprintf("\rT1: %c T2: %c", cOne, cTwo);
IntMasterEnable();
}

unsigned char inbyte;
*/

int
main(void) {
	
	ConfigureUART();
	
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
	                  SYSCTL_XTAL_16MHZ);	
	
	UARTprintf("\n start \n");

	#ifdef Bluetooth_module
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

	GPIOPinConfigure(GPIO_PB0_U1RX);
	GPIOPinConfigure(GPIO_PB1_U1TX);
	GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	
	UARTConfigSetExpClk(UART1_BASE,16000000, 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
	UARTStdioConfig(0, 9600, 16000000);
	UARTprintf("welcome");
	#endif
	//
	// Set up and enable the SysTick timer.  It will be used as a reference
	// for delay loops.  The SysTick timer period will be set up for one
	// millisecond.
	//
	SysTickPeriodSet(SysCtlClockGet()/1000);
	
	SysTickIntEnable();
	SysTickEnable();
	
	#ifdef DC_motors
	
	InitDCMotors();

	#endif
	
	#ifdef ServoMotors 
	
	InitServoMotors();
	ConfigureGripperServo();
	ConfigureElbowServo();
	ConfigureBaseServo();
	SetGripperServoAngle(90);
	SetElbowServoAngle(100);
	SetBaseServoAngle(0);
	
	#endif

	#ifdef Bleutooth_module
	UARTCharPut(UART1_BASE,'s');
  UARTCharPut(UART1_BASE,'t');
	UARTCharPut(UART1_BASE,'a');
	UARTCharPut(UART1_BASE,'r');
	UARTCharPut(UART1_BASE,'t');
	#endif
	
	 while (1)
	 {
		 
		PWMDCMotorLeftFront(100000);
		PWMDCMotorLeftBack(100000);
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1,  GPIO_PIN_1);

		PWMDCMotorRightFront(100000);
		PWMDCMotorLeftBack(100000);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2,GPIO_PIN_2);
		
		#ifdef Flame_sensor
		if (GPIOPinRead(FlameSensor_PORT, Flamemode) == 1)
		{
			
			UARTprintf("there is a fire");
	
		}
		
		else if (GPIOPinRead(FlameSensor_PORT, Flamemode) == 0)
		{
			
			UARTprintf("No fire");
		
		}
		#endif
		
		#ifdef IR_sensor
		
		if (GPIOPinRead(IR_PORT, irmode) == 1)
		{
			
			UARTprintf("there is an obstacle");
	
		}
		
		else if (GPIOPinRead(IR_PORT, irmode) == 0)
		{
			
			UARTprintf("No Obstacle");
		
		}
		
		#endif
		
	 }
		

  return 0;
	 
}
