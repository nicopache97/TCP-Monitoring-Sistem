#include <Adafruit_NeoPixel.h>

#define LED_PIN    5 // GPIO5
#define LED_LENGTH  5 // 5 leds
#define PIN_ADC 32    // Pin para lectura analógica
#define PIN_LED 2       // LED para indicación visual

int i=0;

Adafruit_NeoPixel strip(LED_LENGTH, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  strip.begin();
}

void encenderBlanco(int brillo) {
  if(brillo>255) brillo=255;// trunca de 0 a 255
  if(brillo<0) brillo=0; 
  for(int i=0; i<LED_LENGTH; i++) {
      strip.setPixelColor(i, strip.Color(brillo, brillo, brillo));
  }
  strip.show();
}


void loop() {
  encenderBlanco(i);
  i=(i+1)%255;
  Serial.println(analogRead(PIN_ADC));
  delay(10);
}