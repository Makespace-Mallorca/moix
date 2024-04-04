//#include "CubeCell_NeoPixel.h"
#include "Adafruit_NeoPixel.h"


#define PIN     GPIO1   // Pin driving DIN of first NeoPixel
#define NUMPIXELS 7   // Number of NeoPixels in pixels or ring
#define DELAYVAL 100  // Time (in milliseconds) to pause between pixels
#define BRIGHTNESS 50 // Brightness of NeoPixel

//CubeCell_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ400);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext,LOW); //SET POWER
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'

  pixels.begin(); // This initializes the NeoPixel library. 
  delay(500);
  pixels.fill(pixels.Color(0, 0, 0),0, pixels.numPixels()); // black  
  Serial.println("LightOFF");

  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(500);
  pixels.fill(pixels.Color(255, 255, 255),0, pixels.numPixels()); // black  
  Serial.println("LightOn");
}

uint8_t i=0, j=0;

void loop() {
    pixels.setPixelColor(j, pixels.Color(i, 0, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(200); // Pause before next pass through loop

    pixels.setPixelColor(j, pixels.Color(0, i, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(200); // Pause before next pass through loop

    pixels.setPixelColor(j, pixels.Color(0, 0, i));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(200); // Pause before next pass through loop
    pixels.setPixelColor(j, pixels.Color(0, 0, 0));
    
    i+=10;
    j++;
    if(j>15)
      j=0;

}
