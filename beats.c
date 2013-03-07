/*** PLEASE NOTE THAT THIS WAS DONE IN A FOLDER AT
C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\arm\examples\TexasInstruments\Stellaris\boards\ek-lm3s1968\ ***/

//I regret nothing

#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.c"
#include "driverlib/timer.h"

int main()
{  
   // Set System Clock to divide by 16 (500kHz) and use PLL, using the 8MHz internal crystal.
   SysCtlClockSet(SYSCTL_SYSDIV_16 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_8MHZ);
  
  //Enable Timer0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
   
  // Configure TimerA as a half-width periodic timer.
  TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC));
  
  // Set the count time for timer 0.
  TimerLoadSet(TIMER0_BASE, TIMER_A, 488);

  // Configure the counter (TimerA) to count the positive edges.
  TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
  
  //Enable timeout (countdown to 0) interrupt for TimerA of Timer0.
  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
   
  //Enable GPIO Port D
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  
  //Enable GPIO Port G
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
  
  //Enable Port D, Pin 1 as output
  GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, 0x02);
  
  //Enable POrt G, Pins 1&2 as input
  GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, 0x06);
  
  // Set port D to start with 0.
  GPIOPinWrite(GPIO_PORTD_BASE, 0x02, 0x00);
  
  //
  // Enable the timers.
  //
  TimerEnable(TIMER0_BASE, TIMER_A);
  
  while(1){
    
    //While the timer interrupt for timeout (counted to 0) has not been triggered...
    while(!(TimerIntStatus(TIMER0_BASE, true) & TIMER_TIMA_TIMEOUT))
    {      
      //...chill
    }
    
    //If one button pressed, increase the frequency by 10
    if(GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_1) == 0x02)
      TimerLoadSet(TIMER0_BASE, TIMER_A, (TimerLoadGet(TIMER0_BASE, TIMER_A) + 10));
    
    //If one button pressed, decrease the frequency by 10
    if(GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_2) == 0x04)
      TimerLoadSet(TIMER0_BASE, TIMER_A, (TimerLoadGet(TIMER0_BASE, TIMER_A) - 10));
    
    //Flip the buzzer output
    GPIOPinWrite(GPIO_PORTD_BASE, 0x02, ~GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1));
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
       
  };
  
}
