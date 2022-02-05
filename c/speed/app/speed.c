/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "lwgps/lwgps.h"
#include "lwrb/lwrb.h"
#include <string.h>

#include "EPD_3in7.h"
#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "Debug.h"
#include <stdlib.h> // malloc() free()
#include <math.h>


/// \tag::uart_advanced[]

#define UART_ID uart0
#define BAUD_RATE 115200
#define DEFAULT_BAUD_RATE 9600
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// For the GNSS board
#define UART_TX_PIN 0
#define UART_RX_PIN 1

static int chars_rxed = 0;

// For LwGPS

/* GPS handle */
lwgps_t hgps;

/* GPS buffer */
lwrb_t hgps_buff;
uint8_t hgps_buff_data[12];

// RX interrupt handler
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
	//printf("%c",ch);
        chars_rxed++;
        /* Make interrupt handler as fast as possible */
        /* Only write to received buffer and process later */
        lwrb_write(&hgps_buff, &ch, 1);
    }
}

// Display frame
#define FRAME_MS 2000

volatile bool request_animate = false;

bool set_request_animate(struct repeating_timer *t) {
  request_animate = true;
  return true;
}

const char deg=176; //deg symbol

//Create a new image cache
UBYTE *BlackImage;
UBYTE *CharImage;

#define X_OFFSET 240
#define Y_OFFSET 30
#define CORNER_X 150
#define CORNER_RADIUS 60
#define SIDE_LENGTH 120
#define HALF_BAR 17
#define INNER_DOT_RADIUS 12
#define OUTER_DOT_RADIUS 30
//#define ARC 47
// maximum shift bar is equal to this many degrees:
#define MAX_DEG 48

// maximum shift in pixels we can show
int maxshift() {
        int arc = M_PI_2 * CORNER_RADIUS;
	return CORNER_X + arc + SIDE_LENGTH;
}

// position of the X-coordinate of middle of the bar for a given shift value in pixels
int shiftx(int shift){

    if (shift == 0) {
      return X_OFFSET;
    }

    int abs_shift = abs(shift);
    int direction = (shift > 0)? 1 : -1;

    if (shift <= CORNER_X) {
      return X_OFFSET + (direction * shift);
    }

    float arc = M_PI_2 * CORNER_RADIUS; //quarter circumference 

    if  (shift <= (CORNER_X + (int)arc)) {
      float angle = (float) (shift - CORNER_X) / arc * M_PI_2;
      return X_OFFSET + direction * ( CORNER_X + (int) (CORNER_RADIUS * sinf(angle))); //sinf for float, sin for double
   }

   // vertical line, does not need arc length in it
   return X_OFFSET + direction * ( CORNER_X + CORNER_RADIUS);

}

int shifty(int shift) {

    if (shift <= CORNER_X) {
	return Y_OFFSET;
    }

    float arc = M_PI_2 * CORNER_RADIUS; //quarter circumference 

    if  (shift <= (CORNER_X + (int)arc)) {

      float angle = (float) (shift - CORNER_X) / arc * M_PI_2;

      return Y_OFFSET + (int) (CORNER_RADIUS * (1-cosf(angle))); //sinf for float, sin for double
   }

   // vertical line, does not need arc length in it
   return Y_OFFSET + CORNER_RADIUS + shift - CORNER_X - arc;

}

int pseudoshift;

int shiftdeg() {
	// just for demo purposes
  return (++pseudoshift%90)-45;
}

void animate() {
    char ok;
    ok = (hgps.is_valid) ? 65 : 86;

    //printf("[%c/%f] %02d:%02d:%02d UTC @(%f,%f) z=%fm s=%fKn c=%f%c\n",ok, hgps.dop_h, hgps.hours, hgps.minutes, hgps.seconds, hgps.latitude, hgps.longitude, hgps.altitude, hgps.speed, hgps.course, deg);
    //printf("%02d:%02d\n",hgps.minutes,hgps.seconds);
    char c[2];

    int t = hgps.seconds;

    Paint_NewImage(BlackImage, EPD_3IN7_WIDTH, EPD_3IN7_HEIGHT, 90, WHITE);
    Paint_SelectImage(BlackImage);
    Paint_SetScale(2);
    Paint_Clear(WHITE);
    Paint_DrawCircle(270,265,12, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    // shift indicator

    int shift = shiftdeg() * 480 / MAX_DEG; //intermediate results are int so order matters?
  
    int i;
    for (i=1; i < shift; i++) {
        Paint_DrawCircle(shiftx(i), shifty(i), HALF_BAR, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    }

	printf("%d->%d end@(%d,%d)\n",shiftdeg(), shift, shiftx(shift), shifty(shift));
    // draw line
    //Paint_DrawRectangle(X_OFFSET,Y_OFFSET-HALF_BAR, X_OFFSET+shift, Y_OFFSET+HALF_BAR, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    // draw end
    //Paint_DrawCircle(240+shift,30,17, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    // central starting point
    Paint_DrawCircle(X_OFFSET,Y_OFFSET,OUTER_DOT_RADIUS, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(X_OFFSET,Y_OFFSET,INNER_DOT_RADIUS, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    // draw speed last so corner construction does not blank it out.

    // decimal point
    Paint_DrawCircle(270,265,12, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    sprintf(c,"%02d",t);
    
    for (i = 0; i < 2; i ++) {
        c[i] -=  16;
    }

    if (true) { //when have speed, check if first digit is a 1
    Paint_DrawChar(25,90, 33, &Font189, BLACK, WHITE);
    }

    Paint_DrawString_EN(125,90, c, &Font189, WHITE, BLACK);

 
    EPD_3IN7_1Gray_Display(BlackImage);
    
}

// Ink


int main() {
 
  
     // initialise the USB for printf
    stdio_init_all();
  
    // Set up our UART with a basic baud rate. (TODO - is this step really needed for 9600 baud?)
    uart_init(UART_ID, DEFAULT_BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    //int __unused actual = uart_set_baudrate(UART_ID, DEFAULT_BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

	// TODO - do we enable FIFO to be able to get all the data in each message from the GNSS? Probably ...
    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

    // OK, all set up.
    // Lets send a basic string out, and then run a loop and wait for RX interrupts
    // The handler will count them, but also reflect the incoming data back with a slight change!
    //uart_puts(UART_ID, "\nHello, uart interrupts\n");
    // initialise the ink display
    if(DEV_Module_Init()!=0){
        return -1;
    }

    EPD_3IN7_4Gray_Init();
    EPD_3IN7_4Gray_Clear();
    DEV_Delay_ms(500);


  /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_3IN7_WIDTH % 4 == 0)? (EPD_3IN7_WIDTH / 4 ): (EPD_3IN7_WIDTH / 4 + 1)) * EPD_3IN7_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

     Paint_NewImage(BlackImage, EPD_3IN7_WIDTH, EPD_3IN7_HEIGHT, 90, WHITE);
     Paint_SelectImage(BlackImage);
     Paint_SetScale(4);
     Paint_Clear(WHITE);
     //Paint_DrawString_EN(200,75, "SPEED-SHIFT", &Font24, WHITE, BLACK);
     //Paint_DrawString_EN(200,95, "    v0.1", &Font24, WHITE, BLACK);

     EPD_3IN7_4Gray_Display(BlackImage);
     DEV_Delay_ms(1000);

     EPD_3IN7_4Gray_Clear();
     DEV_Delay_ms(500);

     EPD_3IN7_1Gray_Init();
     EPD_3IN7_1Gray_Clear();


	// lwGPS setup
	uint8_t rx;

    /* Init GPS */
    lwgps_init(&hgps);

    /* Create buffer for received data */
    lwrb_init(&hgps_buff, hgps_buff_data, sizeof(hgps_buff_data));

	// Setup timer for managing refresh of the display every 1second
	struct repeating_timer timer;
        add_repeating_timer_ms(FRAME_MS, set_request_animate, NULL, &timer);




    // Change baudrate
    char changebaud[] = "$PQBAUD,W,115200*43\r\n";
    uart_puts(UART_ID, changebaud);
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);


	while (1) {

        /* Process all input data */
        /* Read from buffer byte-by-byte and call processing function */
        if (lwrb_get_full(&hgps_buff)) {        /* Check if anything in buffer now */
            while (lwrb_read(&hgps_buff, &rx, 1) == 1) {
                lwgps_process(&hgps, &rx, 1);   /* Process byte-by-byte */
            }
        } else if  (request_animate > 0 ){
			request_animate = false;
			animate();
		}
	}

}

/// \end:uart_advanced[]
