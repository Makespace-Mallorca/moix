// ***************************************************************************
// INCLUDES PARA MATRIZ DE LEDS RGB
// ***************************************************************************
#include <Adafruit_NeoPixel.h>

#define LED_PIN 13   // PIN al que conectamos la Matriz

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB/*+ NEO_KHZ800*/);

void initilizeRGBMatrix() 
{ 
  Serial.println("RGB Matrix");
  
  strip.begin();

// Si nos despertamos por que no se escucha ruido, no queremos hacer el efecto de luces
  if(sleepByNoise) {
    boardLED_Blink(3);
    return;
  }

  
  //strip.show();
  
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
    strip.show();

  for(int k=0;k<3;k++)
  {

    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0));
      strip.show();
      delay(200);
    }
  
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
      strip.show();
  }
}
void turnLedsOff()
{
  for (int i = 0; i <NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }  
    delay(200);
      strip.show();
}

// Establece el valor de cada led
void setRGBColor(int r,int g,int b)
{

  for (int i = 0; i <NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();
    delay(40);
  }
  for (int i = 0; i <NUM_LEDS; i++) {

    strip.setPixelColor(i, strip.Color(r*LED_STRENGTH, g*LED_STRENGTH, b*LED_STRENGTH));
    strip.show();
    delay(40);
  }

}

void rainbowEffect()
{
}
