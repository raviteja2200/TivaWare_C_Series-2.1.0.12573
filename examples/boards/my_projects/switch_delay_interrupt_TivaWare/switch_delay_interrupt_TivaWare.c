#include <stdint.h>
#include <stdbool.h>
#include "switch_delay_interrupt_TivaWare.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c1294ncpdt.h"



#define 	BLUE_MASK 		0x04
//*****************************************************************************
//
//!
//! A very simple example that interfaces with the LED D1 (PN1) and SW1 (PJ0) 
//! using direct register access. When SW2 is pressed, the LED is turned on. When 
//! SW1 is released, the LED is turned off. Interrupt on PF0 is configured as 
//! edge-triggered on both edges
//
//*****************************************************************************


void
PortFunctionInit(void)
{
    //
    // Enable Peripheral Clocks 
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    //
    // Enable pin PJ0 for GPIOInput
    //
    GPIOPinTypeGPIOInput(GPIO_PORTJ_AHB_BASE, GPIO_PIN_0);

    //
    // Enable pin PN1 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
	
		//*************************************************************
		//	The code above is automatically generated by PinMux
		//  The code below should be manually added by the programmer
		//*************************************************************
		
		//Enable pull-up on PJ0
		GPIO_PORTJ_AHB_PUR_R |= 0x01; 
}


void
Interrupt_Init(void)
{
  IntEnable(INT_GPIOJ);  							// enable interrupt 51 in NVIC (GPIOJ)
	IntPrioritySet(INT_GPIOJ, 0x00); 		// configure GPIOJ interrupt priority as 0
	GPIO_PORTJ_AHB_IM_R |= 0x01;   		// arm interrupt on PJ0
	GPIO_PORTJ_AHB_IS_R &= ~0x01;     // PJ0 is edge-sensitive
  GPIO_PORTJ_AHB_IBE_R |= 0x01;   	// PJ0 both edges trigger 
  //GPIO_PORTJ_AHB_IEV_R &= ~0x01;  // PJ0 falling edge event
	IntMasterEnable();        		// globally enable interrupt
}

//interrupt handler
void GPIOPortJ_Handler(void){
	
	// acknowledge flag for PJ0
  GPIOIntClear(GPIO_PORTJ_AHB_BASE, GPIO_PIN_0);       
	
	//SW1 is pressed
  if((GPIO_PORTJ_AHB_DATA_R&0x01)==0x00) 
	{
			// Turn on the LED.
			GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x02);
	}
	else
	{
			// Turn off the LED.
			GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x00);
	}
}

int main(void)
{
	
		//initialize the GPIO ports	
		PortFunctionInit();
		
	//configure the GPIOJ interrupt
		Interrupt_Init();
	
    //
    // Loop forever.
    //
    while(1)
    {

      // Delay for a bit.
				SysCtlDelay(6000000);	
    }
}