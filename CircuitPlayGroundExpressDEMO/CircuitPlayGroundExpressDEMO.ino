#define USE_ARDUINO_INTERRUPTS true 
#define COLORRED         0xFF0000   // change this to your favorite color
#define COLORORANGE   0xFFA500
#define COLORBLUE     0x0000FF
#define FLASH_RATE    250        // lower is faster


#include <Adafruit_CircuitPlayground.h>
#include <PulseSensorPlayground.h> 
#include <Wire.h>
#include <SPI.h>

bool sound = true;
float X, Y, Z;
const uint8_t spDANGER[]        PROGMEM = {0x2D,0xBF,0x21,0x92,0x59,0xB4,0x9F,0xA2,0x87,0x10,0x8E,0xDC,0x72,0xAB,0x5B,0x9D,0x62,0xA6,0x42,0x9E,0x9C,0xB8,0xB3,0x95,0x0D,0xAF,0x14,0x15,0xA5,0x47,0xDE,0x1D,0x7A,0x78,0x3A,0x49,0x65,0x55,0xD0,0x5E,0xAE,0x3A,0xB5,0x53,0x93,0x88,0x65,0xE2,0x00,0xEC,0x9A,0xEA,0x80,0x65,0x82,0xC7,0xD8,0x63,0x0A,0x9A,0x65,0x5D,0x53,0xC9,0x49,0x5C,0xE1,0x7D,0x2F,0x73,0x2F,0x47,0x59,0xC2,0xDE,0x9A,0x27,0x5F,0xF1,0x8B,0xDF,0xFF,0x03};
// CircuitPlayground.speaker.say(spDANGER);
// we light one pixel at a time, this is our counter
uint8_t pixeln = 0;

volatile boolean QS = false; // becomes true when Arduoino finds a beat.

int PulseSensorPurplePin = A2;        // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0
int LED13 = 4;   //  The on-board Arduion LED

int Signal;                // holds the incoming raw data. Signal value can range from 0-1024
int Threshold = 550;            // Determine which Signal to "count as a beat", and which to ingore.

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Circuit Playground test!");

  pinMode(LED_BUILTIN, OUTPUT);
  CircuitPlayground.begin();
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
}

void loop() {
  
  // put your main code here, to run repeatedly:
  if (CircuitPlayground.leftButton()) {
    Serial.println("Sound On (LEFT)");
    sound = true;
  } 
  if (CircuitPlayground.rightButton()){
    Serial.println("Sound Off (RIGHT)");
    sound = false;
    CircuitPlayground.speaker.say(spDANGER);
  }
  
  // ============[TEMP]==================

  Serial.print("Temperature ");
  Serial.print(CircuitPlayground.temperature());
  Serial.println(" *C");

  
  // =============[ACEL]==================

  //************ TEST ACCEL
  // Display the results (acceleration is measured in m/s*s)
  Serial.print("X: "); Serial.print(CircuitPlayground.motionX());
  Serial.print(" \tY: "); Serial.print(CircuitPlayground.motionY());
  Serial.print(" \tZ: "); Serial.print(CircuitPlayground.motionZ());
  Serial.println(" m/s^2");
  
  // =============[PULSE]==================
  
  /*
  Signal = analogRead(PulseSensorPurplePin);  // Read the PulseSensor's value.
                                              // Assign this value to the "Signal" variable.
  Serial.print("BPM: ");
  Serial.println(Signal);                    // Send the Signal value to Serial Plotter.


  if(Signal > Threshold){                          // If the signal is above "550", then "turn-on" Arduino's on-Board LED.
    digitalWrite(LED13,HIGH);
  } else {
    digitalWrite(LED13,LOW);                //  Else, the sigal must be below "550", so "turn-off" this LED.
  }

  int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 

  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
    Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
    Serial.print("BPM: ");                        // Print phrase "BPM: " 
    Serial.println(myBPM);                        // Print the value inside of myBPM. 
  }

  /*
  if (QS == true){ // A Heartbeat Was Found
    // BPM and IBI have been Determined
    // Quantified Self "QS" true when arduino finds a heartbeat
    Serial.print("Your Beat ");
    Serial.println(BPM);
    QS = false; // reset the Quantified Self flag for next time
  }*/

  // ==============[CONDITIONAL FLAGS]===========

  if(CircuitPlayground.rightButton()){
    if(CircuitPlayground.leftButton()){
      for (int pixel=0; pixel<10; pixel++) {
        CircuitPlayground.setPixelColor(pixel, COLORORANGE); // Moderate Temp
      }
    }
  }
  
  testTemp();
  AlertAcel();
  delay(20);
}

void AlertAcel(){
  float tmpX, tmpY;
  X = CircuitPlayground.motionX();

  delay(1250);
  tmpX = CircuitPlayground.motionX();

  if(abs(X-tmpX) > 3.5){
    for (int pixel=0; pixel<10; pixel++) {
      CircuitPlayground.setPixelColor(pixel, COLORBLUE);
    }
    delay(FLASH_RATE);

    // Turn off all the NeoPixels
    CircuitPlayground.clearPixels();

    // Leave them off for a little bit
    delay(FLASH_RATE);
    if(sound){
      CircuitPlayground.speaker.say(spDANGER);
    }
  }
}

void testTemp(){
  if(CircuitPlayground.temperature() < 20 && CircuitPlayground.temperature() > 22.22) {
    Serial.println("Current Temp: ");
    Serial.print(CircuitPlayground.temperature());
    Serial.print('\n');
    // Turn on all the pixels to COLOR
    for (int pixel=0; pixel<10; pixel++) {
      CircuitPlayground.setPixelColor(pixel, COLORORANGE);    
    }

    // Leave them on for a little bit  
    delay(FLASH_RATE);

    // Turn off all the NeoPixels
    CircuitPlayground.clearPixels();

    // Leave them off for a little bit
    delay(FLASH_RATE);
    }

  if(CircuitPlayground.temperature() < 17.22 || CircuitPlayground.temperature() > 25){
    for (int pixel=0; pixel<10; pixel++) {
      CircuitPlayground.setPixelColor(pixel, COLORRED);
    }
    
    delay(FLASH_RATE);
    CircuitPlayground.clearPixels();
    delay(FLASH_RATE);
  }
}
