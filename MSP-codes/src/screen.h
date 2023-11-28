/*
This file include the header for the management of the on-screen feedback
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <stdio.h>
#include <msp.h>
#include <grlib.h>
#include <math.h>
#include <Crystalfontz128x128_ST7735.h>
#include <HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h>

#define bgColor GRAPHICS_COLOR_BLACK         // background color, CURRENT: Black
#define fgColor GRAPHICS_COLOR_GOLDENRON     // foreground color, CURRENT: Goldenron
#define font    g_sFontCmss18b               // font type, CURRENT: Cmss 18px Bold
#define startX  11                           // progress bar first pixel on X axis
#define startY  87                           // progress bar first pixel on Y axis

Graphics_Context context; // Screen context used for all feedback
uint16_t vectorsDrawn;    // Number of vectors drawn, updated each time function "updateScreen" is called
bool isDrawing;           // Tracks if the d
uint8_t prevPercentage;   // 

void init_screen(void);   /* Basic screen setup */
void updateScreen(void);  /* Main function recalled by interrupt handler */

#endif
