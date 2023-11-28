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
    int i;
    for(i = 0; i < 1000; i++){
        __delay_cycles(10000);
        updateScreen();
    }
   
}

// DEMO: needs to be changed
void drawInterface(void){
    Graphics_clearDisplay(&context);
    Graphics_flushBuffer(&context);
    const Graphics_Rectangle motor1 = {10, 10, 30, 30};
    const Graphics_Rectangle motor2 = {74, 10, 94, 30};
    Graphics_fillRectangle(&context, &motor1);
    Graphics_drawStringCentered(&context, (int8_t*) ":", AUTO_STRING_LENGTH, 40, 18, OPAQUE_TEXT);
    Graphics_fillRectangle(&context, &motor2);
    Graphics_drawStringCentered(&context, (int8_t*) ":", AUTO_STRING_LENGTH, 104, 18, OPAQUE_TEXT);
    Graphics_drawImage(&context, &DrawingBar, 0, 65);
    //Graphics_drawStringCentered(&context, &vectorsDrawn, AUTO_STRING_LENGTH, 64, 115, OPAQUE_TEXT);
}

void updateScreen(void){
    if(!isDrawing){
        isDrawing = true;
        drawInterface();
    }
    uint8_t startX = 8;
    uint8_t startY = 64 + 23;
    uint16_t numVec = 1000; // dummy num of vectors
    vectorsDrawn++;
    uint8_t percentage = floor(vectorsDrawn * 100 / numVec);
    int8_t buffer[5];
    snprintf((char*)buffer, sizeof(buffer), "%u", percentage);
    Graphics_Rectangle rect = {0, 105, 70, 128};
    if(prevPercentage != percentage) {
        prevPercentage = percentage;
        Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &rect, 0xFFFF);
    }
    Graphics_drawStringCentered(&context, &buffer[0], AUTO_STRING_LENGTH, 60, 115, OPAQUE_TEXT);
}