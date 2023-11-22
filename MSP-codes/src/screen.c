#include "screen.h"

// Screen context used for all feedback
Graphics_Context context; 
const Graphics_Font *font = &g_sFontCmss14b;

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
    // Font for text design - currently set to Cmss 14px Bold
    */ 
    Graphics_setForegroundColor(&context, GRAPHICS_COLOR_GOLDENRON);
    Graphics_setBackgroundColor(&context, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&context, font);

    // Make sure nothing is cached for display
    Graphics_clearDisplay(&context);
    Graphics_flushBuffer(&context);

    // Bound to be removed, for testing of font / colors / proportions
    Graphics_drawStringCentered(&context, (int8_t *) "Silent Leges", AUTO_STRING_LENGTH, 64, 64, OPAQUE_TEXT);
    Graphics_drawStringCentered(&context, (int8_t *) "Inter Arma 123", AUTO_STRING_LENGTH, 64, 64 + (int32_t) Graphics_getFontHeight(font), OPAQUE_TEXT);


}