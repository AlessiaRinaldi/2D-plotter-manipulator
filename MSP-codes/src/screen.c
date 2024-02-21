#include "screen.h"
#include "pictures/images.h"


void init_screen(void){

    // Initialize screen SPI communication
    Crystalfontz128x128_Init();

    // Set screen orientation, UP implying joystick positioned on the left side of screen
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // Set grlib to use CrystalFontz function library
    Graphics_initContext(&context, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);

    /* Set screen characteristics
    // Foreground Color for text and drawings on screen
    // Background Color for any untouched pixel
    // Font for text design - currently set to Cmss 18px Bold
    */ 
    Graphics_setForegroundColor(&context, fgColor);
    Graphics_setBackgroundColor(&context, bgColor);
    Graphics_setFont(&context, &font);

    vectorsDrawn = 0;
    isDrawing = false;
    prevPercentage = 0;
    numVec = 0;
    
    // Make sure nothing is cached for display
    Graphics_clearDisplay(&context);
    Graphics_flushBuffer(&context);

    // Draws default booting image
    Graphics_drawImage(&context, &Default, 0, 0);

}

// Updates screen percentage if enough vectors have been drawn to increase it
void updateScreen(void){
    if(!isDrawing){
        isDrawing = true;
        Graphics_clearDisplay(&context);
        Graphics_flushBuffer(&context);
        Graphics_drawImage(&context, &DrawingBar, 0, 35);
        int8_t buffer[5];
        snprintf((char*)buffer, sizeof(buffer), "%u", (uint8_t) 0);
        Graphics_drawStringCentered(&context, &buffer[0], AUTO_STRING_LENGTH, 57, 85, OPAQUE_TEXT);
    }
    vectorsDrawn++;
    uint8_t percentage = floor(vectorsDrawn * 100 / numVec);

    if(prevPercentage != percentage) {
        
        int8_t buffer[5];
        snprintf((char*)buffer, sizeof(buffer), "%u", percentage);
        Graphics_Rectangle rect = {30, 75, 68, 98};
        Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &rect, bgColor);
        if (percentage - prevPercentage == 1) {
            Graphics_drawVerticalLineOnDisplay(&g_sCrystalfontz128x128, startX + percentage, startY, startY + 14, 0x07e0);
        } else {
            Graphics_Rectangle rec = {startX + prevPercentage + 1, startY, startX + percentage, startY + 14};
            Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &rec, 0x7e0);
        }
        if (percentage == 100) {
            Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &(Graphics_Rectangle){startX + percentage +1, startY, startX + percentage + 4, startY + 14}, 0x7e0);
            Graphics_flushBuffer(&context);
        }
        Graphics_drawStringCentered(&context, &buffer[0], AUTO_STRING_LENGTH, 57, 85, OPAQUE_TEXT);
        prevPercentage = percentage;
    }
}