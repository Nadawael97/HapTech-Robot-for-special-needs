#include <stdint.h>
#include <stdlib.h>
#include "TM4C123GH6PM.h"
#include "my_headers/UART0.h"
#include "my_headers/gpio.h"
#include "my_headers/delay.h"
#include <string.h>
 
#define BT_out GPIOB->DATA
#define LED1			 1
#define LED2 			 2
#define LED3 			 4
#define FAN 			 8
#define LIGHT			 16
 
 
void BT_PORT_init(void){
		volatile uint32_t delay=0;//Declaring  dummy bit for waiting after enabling clock  
		SYSCTL->RCGC2=0x00000002; //Clock for the PORTB
		delay=SYSCTL->RCGC2;	  //waiting after enabling the clock
		GPIOB->LOCK=0x4C4F434B;	  //writing the magic number to LOCK register	
		GPIOB->CR=0x1F;		  //Allowing changes for bits PB0 to PB4 
		GPIOB->DIR=0x1F;	  //	Setting the direction IO pins
		GPIOB->DEN=0x1F;	  //	Enabling digital pins from 0 to 4
}
 
int main(){			
	SystemInit(); 
	BT_PORT_init();
	UART0_init(9600); //initializing the Bluetooth port
	char *target;   //char pointer to point the string 
	int size=10;    //max size of string 
	while(1){
		target=U0_get_str(target,10); //getting the data from HC-05 module 
else if (!strcmp("FAN",target))BT_out^=FAN;  //comparing it with the FAN string and switching
else if (!strcmp("LIGHT",target))BT_out^=LIGHT;//same for the LIGHT and so on
else if (!strcmp("LED1",target))BT_out^=LED1;
else if (!strcmp("LED2",target))BT_out^=LED2;
else if (!strcmp("LED3",target))BT_out^=LED3;
		free(target);
}
	
}