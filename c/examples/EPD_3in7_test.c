/*****************************************************************************
* | File      	:   EPD_3IN7_test.c
* | Author      :   Waveshare team
* | Function    :   3.7inch e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-07-16
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_Test.h"
#include "EPD_3in7.h"

int EPD_3in7_test(void)
{
    printf("EPD_3IN7_test LARGE FONT\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
    EPD_3IN7_1Gray_Init();
    EPD_3IN7_1Gray_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_3IN7_WIDTH % 4 == 0)? (EPD_3IN7_WIDTH / 4 ): (EPD_3IN7_WIDTH / 4 + 1)) * EPD_3IN7_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_3IN7_WIDTH, EPD_3IN7_HEIGHT, 90, WHITE);
    Paint_SetScale(2);
    Paint_Clear(WHITE);

#if 0 // Try large font

    //Paint_DrawString_EN(10, 90, "! ,", &Font200, GRAY1, BLACK);
    Paint_DrawString_EN(10, 10, "&", &Font200, WHITE, BLACK);

    EPD_3IN7_1Gray_Display(BlackImage);

    DEV_Delay_ms(1000);

    Paint_Clear(WHITE);

    Paint_DrawString_EN(10, 90, "%&", &Font200, WHITE, BLACK);

    EPD_3IN7_1Gray_Display(BlackImage);

    DEV_Delay_ms(1000);

#endif 

#if 0 // Drawing on the image, partial display
    //1.Select Image
    printf("SelectImage:BlackImage\r\n");   
    Paint_SelectImage(BlackImage);
    Paint_SetScale(4);
    Paint_Clear(WHITE);

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(10, 0, "G'day!", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "Greetings world", &Font12, WHITE, BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
    Paint_DrawString_EN(10, 150, "HELLO THERE!!", &Font24, BLACK, GRAY1);
    Paint_DrawString_EN(10, 175, "GOO with white background", &Font24, WHITE, GRAY2);
    Paint_DrawString_EN(10, 200, "BAR with white background", &Font24, WHITE, GRAY3);
    Paint_DrawString_EN(10, 225, "FOO white background", &Font24, WHITE, GRAY4);
    printf("EPD_Display\r\n");
    EPD_3IN7_4Gray_Display(BlackImage);
    DEV_Delay_ms(4000);
#endif

#if 1 // partial update, just 1 Gray mode

    const char *a[5];
    a[0] = "\")$";
    a[1] = "\")&";
    a[2] = "\")(";
    a[3] = "# !";
    a[4] = "# #";

    int i;

    EPD_3IN7_1Gray_Init();       //init 1 Gray mode
    EPD_3IN7_1Gray_Clear();

    for (i = 0; i <5; i++) { 
    Paint_NewImage(BlackImage, 270, 450, 90, WHITE);
    Paint_SelectImage(BlackImage);
    Paint_SetScale(2);
    Paint_Clear(WHITE);
    //Paint_DrawRectangle(1, 1, 450, 200, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    //Paint_DrawRectangle(3,3,448,198, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(5,75, a[i], &Font189, WHITE, BLACK);
    printf("Part refresh...\r\n");
    Paint_DrawRectangle(1, 1, 450, 40, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(3, 3, 448, 38, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);

    if ( i > 0) {
    Paint_DrawRectangle(240,3,260,38, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
	}
    if  (i > 1) {
       Paint_DrawRectangle(265,3,285,38, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    } 
    if  (i > 2) {
       Paint_DrawRectangle(290,3,310,38, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    } 
    if  (i > 3) {
       Paint_DrawRectangle(315,3,335,38, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    } 
   if  (i > 4) {
       Paint_DrawRectangle(340,3,365,38, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    } 

    EPD_3IN7_1Gray_Display_Part(BlackImage, 8, 8, 278, 458); // Xstart must be a multiple of 8


    //EPD_3IN7_1Gray_Display_Part(BlackImage, 8, 208, 208, 408); 
    DEV_Delay_ms(1000);
}
    

#endif

    EPD_3IN7_4Gray_Init();
    printf("Clear...\r\n");
    EPD_3IN7_4Gray_Clear();

    // Sleep & close 5V
    printf("Goto Sleep...\r\n");
    EPD_3IN7_Sleep();

    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();

    return 0;
}

