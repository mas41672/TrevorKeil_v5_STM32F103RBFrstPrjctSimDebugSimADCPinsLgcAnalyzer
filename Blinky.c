/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher for MCBSTM32E
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "STM32F10x.h"    // Device header
#include <stdio.h>

#include "Board_LED.h"          // Keil::Board Support:MCBSTM32E:LED
#include "Board_ADC.h"          // Keil::Board Support:MCBSTM32E:A/D Converter

uint32_t LEDOn, LEDOff;


/*----------------------------------------------------------------------------
  SysTick IRQ Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void) {
  static uint32_t ticks;
  
  switch (ticks++) {
    case  0: LEDOn  = 1; break;
    case  5: LEDOff = 1; break;
    case  9: ticks  = 0; break;
    
    default:
      if (ticks > 10) {
        ticks = 0;
      }
  }
}

/*----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/

 int32_t adc = -1, adcVal=0;
int main (void) {
  int32_t max_num = LED_GetCount () - 1;
  int32_t num = 0;
  int32_t dir = 1;
 
  char text[5] = "";

  SysTick_Config(SystemCoreClock/100);      /* Generate interrupt each 10 ms  */
  LED_Initialize();                         /* LED Initialization             */
     
  ADC_Initialize();                         /* A/D Converter Init             */
 
  while (1) {
    if (LEDOn) {
      LEDOn = 0;
      LED_On (num);                         /* Turn specified LED on          */
      
    }

    if (LEDOff) {
      LEDOff = 0;
      LED_Off (num);                        /* Turn specified LED off         */
      

      num += dir;                           /* Change LED number              */
      
      if (dir == 1 && num == max_num) {
        dir = -1;                           /* Change direction to down       */
      }
      else if (num == 0) {
        dir =  1;                           /* Change direction to up         */
      }
    }
    
    ADC_StartConversion();                  /* Show A/D conversion bargraph   */
    adcVal = (adc + ADC_GetValue()) >> 1;
    if (adc ^ adcVal) {
      adc = adcVal;
    //  printf(text, "%i ", adcVal);	
      
    }
  }
}





