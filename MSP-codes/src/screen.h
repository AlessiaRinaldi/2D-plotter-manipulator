/*
This file include the header for the management of the on-screen feedback
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <msp.h>
#include <grlib.h>
#include <Crystalfontz128x128_ST7735.h>
#include <HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h>

#define bgColor 0x00000000
#define fgColor 0x00DAA520
#define font    g_sFontCmss14b

// Screen context used for all feedback
Graphics_Context context; 

void init_screen(void); /* Basic screen setup */
void drawDefault(void); /* Draws default screen */
void drawInterface(void); /* Draws main interface */
void updateScreen(void); /* Main function recalled by interrupt handler */

#endif
