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

    /*
    // Bound to be removed, for testing of font / colors / proportions
    Graphics_drawStringCentered(&context, (int8_t *) "Silent Leges", AUTO_STRING_LENGTH, 64, 64 - (int32_t) Graphics_getFontHeight(font) / 2, OPAQUE_TEXT);
    Graphics_drawStringCentered(&context, (int8_t *) "Inter Arma 123", AUTO_STRING_LENGTH, 64, 64 + (int32_t) Graphics_getFontHeight(font) / 2, OPAQUE_TEXT);
    */

   drawDefault();

}

void drawDefault(void){
    // Make sure nothing is cached for display
    Graphics_clearDisplay(&context);
    Graphics_flushBuffer(&context);

    // Draws default booting image
   Graphics_drawImage(&context, &Default, 0, 0);
   drawInterface();
}

// DEMO: needs to be changed
void drawInterface(void){
    Graphics_clearDisplay(&context);
    Graphics_flushBuffer(&context);
    const Graphics_Rectangle motor1 = {10, 10, 30, 30};
    const Graphics_Rectangle motor2 = {10, 50, 30, 70};
    Graphics_fillRectangle(&context, &motor1);
    Graphics_drawStringCentered(&context, (int8_t*) ":", AUTO_STRING_LENGTH, 40, 18, OPAQUE_TEXT);
    Graphics_fillRectangle(&context, &motor2);
    Graphics_drawStringCentered(&context, (int8_t*) ":", AUTO_STRING_LENGTH, 40, 58, OPAQUE_TEXT);
    Graphics_drawImage(&context, &DrawingBar, 0, 65);
    Graphics_drawStringCentered(&context, (int8_t*) "0", AUTO_STRING_LENGTH, 64, 115, OPAQUE_TEXT);
}

void updateScreen(void){
    int numVec = 1000; // dummy num of vectors
}