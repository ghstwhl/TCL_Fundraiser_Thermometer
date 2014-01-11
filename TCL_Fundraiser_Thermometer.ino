/*****************************************************************************
 * TCL_Fundraiser_Thermometer.ino
 *
 * Copyright 2014 Chris O'Halloran
 * Derived from code Copyright 2011-2012 Christopher De Vries
 * This program is distributed under the Artistic License 2.0
 ****************************************************************************/
#include <SPI.h>
#include <TCL.h>

const int LEDS = 25; // The number of LEDs in the TCL strand.

/* Current values for the pixels are stored in the following three integers */
int red_values;
int green_values;
int blue_values;

/* global value for the height of the lit thermometer */
int THERM_HEIGHT;

void setup() {
  int i;

  TCL.begin(); // Begin protocol for communicating with the TCL strand
  TCL.setupDeveloperShield(); // Set up developer shield for inputs
  Serial.begin(9600); // Start serial communication at 9600 baud

  /* Start by initializing all pixels to black */
  for(i=0;i<LEDS;i++) {
    red_values=0;
    green_values=0;
    blue_values=0;
  }

  update_strand(); // Send the black pixels to the strand to turn off all LEDs.
}

void loop() {
    
    /* Read the current red value from potentiometer 0 */
    red_values=map(analogRead(TCL_POT1), 0, 1023, 0, 255);
    
    /* Read the current green value from potentiometer 1 */
    green_values=map(analogRead(TCL_POT2), 0, 1023, 0, 255);

    /* Read the current blue value from potentiometer 2 */
    blue_values=map(analogRead(TCL_POT3), 0, 1023, 0, 255);

    /* Read the thermometer height from potentiometer 3 */
    /* If you are paying attention, you will notice that (LEDS+1) isn't the 'right' value. */
    /* The high range on the map() should be 0,LEDS, but I cheated to compensate for an occasional */
    /* issue with mapping potentiometer values off the arduino TCL dev sheield */
    THERM_HEIGHT=map(analogRead(TCL_POT4), 0, 1023, -1, (LEDS+1) );


    update_strand(); // Send all the pixels out

}

void update_strand() {
  int i;

  TCL.sendEmptyFrame();

  for(i=1;i<=LEDS;i++) {
    if ( i <= THERM_HEIGHT ) {
      TCL.sendColor(red_values,green_values,blue_values);
    }
    else {    
      TCL.sendColor(0,0,0);
    }
  }

  TCL.sendEmptyFrame();
}
