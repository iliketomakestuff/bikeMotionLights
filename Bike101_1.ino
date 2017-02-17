/*
This is PROTOTYPE code in it's simplest form.  It was built on top of example code form Intel (Arduino 101/Curie).
This code powers the lighting project described at http://www.iliketomakestuff.com/make-motion-activated-bike-lighting

Feel free to do whatever you'd like with it, but I cannot provide support for getting it to work.
-Bob

*/
/*
   Copyright (c) 2016 Intel Corporation.  All rights reserved.
   See the bottom of this file for license terms.
*/

/*
   This sketch example demonstrates how the BMI160 accelerometer on the
   Intel(R) Curie(TM) module can be used to detect motion events
*/

#include "CurieIMU.h"
#include <Adafruit_NeoPixel.h>

#define PIN      6
#define N_LEDS 144
#define TAIL_COUNT 10

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

bool blinkState = 0;
int delayval = 0;
int stillTime = 3000; // 3 seconds
unsigned long loopTime = 0;          // get the time since program started
unsigned long interruptsTime = 0;    // get the time when motion event is detected
void setup() {
  Serial.begin(9600); // initialize Serial communication
  //while(!Serial) ;    // wait for Serial port to connect.

  /* Initialise the IMU */
  CurieIMU.begin();
  CurieIMU.attachInterrupt(eventCallback);

  /* Enable Motion Detection */
  CurieIMU.setDetectionThreshold(CURIE_IMU_MOTION, 20); // 20mg
  CurieIMU.setDetectionDuration(CURIE_IMU_MOTION, 10);  // trigger times of consecutive slope data points
  CurieIMU.interrupts(CURIE_IMU_MOTION);

  Serial.println("IMU initialisation complete, waiting for events...");
  strip.begin();
  blinkStartup();
  strip.show();
  
}

void loop() {
  // if motion is detected in 1000ms, LED will be turned up
  loopTime = millis();
  if(abs(loopTime -interruptsTime) < stillTime ) {
    setWhite(60);  //value here is 0-100 strength
    blinkState = true;
  }else {
    blinkState = false;
  }
  if(!blinkState){
    setAmber();
  }
  digitalWrite(13, blinkState);
}


static void eventCallback(void){
  //This is just in place for reference, it serves no purpose in the code
  if (CurieIMU.getInterruptStatus(CURIE_IMU_MOTION)) {
    if (CurieIMU.motionDetected(X_AXIS, POSITIVE))
      Serial.println("Negative motion detected on X-axis");
    if (CurieIMU.motionDetected(X_AXIS, NEGATIVE))
      Serial.println("Positive motion detected on X-axis");
    if (CurieIMU.motionDetected(Y_AXIS, POSITIVE))
      Serial.println("Negative motion detected on Y-axis");
    if (CurieIMU.motionDetected(Y_AXIS, NEGATIVE))
      Serial.println("Positive motion detected on Y-axis");
    if (CurieIMU.motionDetected(Z_AXIS, POSITIVE))
      Serial.println("Negative motion detected on Z-axis");
    if (CurieIMU.motionDetected(Z_AXIS, NEGATIVE))
      Serial.println("Positive motion detected on Z-axis");

    //This DOES serve a purpose: keeping track of the "timeout" for the motion activation.  
    interruptsTime = millis(); 
  } 
}

static void blinkStartup(){
  for(uint16_t i=0; i<3; i++) {
      setAmber();
      strip.show();
      delay(5);
      clearLights();
      strip.show();
      delay(5);
  }
  setAmber();
}
  
static void setStripColor(uint32_t c) {
  
  //set main strip color, minus the tail section
  for(uint16_t i=0; i<N_LEDS-TAIL_COUNT; i++) {
      strip.setPixelColor(i  , c);
      strip.show();
      delay(1);
  }
  //set tail section to red
  for(uint16_t i=N_LEDS-TAIL_COUNT; i<N_LEDS; i++) { 
      strip.setPixelColor(i  , 200,0,0); // red
      strip.show();
      delay(1);
  }
}

//quick functions for specific colors
static void setWhite(int power){
  int p = map(power, 0, 100, 0, 255);
  setStripColor(strip.Color(p, p, p));
}
static void setAmber(){
  setStripColor(strip.Color(10, 2, 0));
}
static void clearLights(){
  setStripColor(strip.Color(0, 0, 0));
}

/*
   Copyright (c) 2016 Intel Corporation.  All rights reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/
