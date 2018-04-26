#define OCTAVE 1

#include <Wire.h>
#include <FFT.h>
#include <Adafruit_NeoPixel.h>
#include "Adafruit_TCS34725.h"

#define HEIGHT 10
#define WIDTH 10
#define LED_PIN 6
#define LIGHT_PIN 5
#define DELAY 10000

#define NOISE_REDUCTION 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(HEIGHT * WIDTH, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
int iter = 0;
int rSum = 0;
int gSum = 0;
int bSum = 0;

uint16_t clear, red, green, blue;

int redOut;
int greenOut;
int blueOut;
int d = 55;

void setup()
{
  Serial.begin(115200); // use the serial port
  strip.begin();
  tcs.begin();

  TIMSK0 = 0;    // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40;  // use adc0
  DIDR0 = 0x01;  // turn off the digital input for adc0
}

void loop()
{
  tcs.setInterrupt(false); 
  delay(d);
  tcs.getRawData(&red, &green, &blue, &clear);
  int upperBound = 1500;
  redOut = constrain(map(red, 0, upperBound, 0, 255), 0, 255);
  greenOut = constrain(map(green, 0, upperBound, 0, 255), 0, 255);
  blueOut = constrain(map(blue, 0, upperBound, 0, 255), 0, 255);

  for (int i = 0; i < HEIGHT * WIDTH; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }

  // I don't know what this code does but it works.
  cli(); // UDRE interrupt slows this way down on arduino1.0
  for (int i = 0; i < 512; i += 2)
  { // save 256 samples
    while (!(ADCSRA & 0x10))
      ;            // wait for adc to be ready
    ADCSRA = 0xf5; // restart adc
    byte m = ADCL; // fetch adc data
    byte j = ADCH;
    int k = (j << 8) | m; // form into an int
    k -= 0x0200;          // form into a signed int
    k <<= 6;              // form into a 16b signed int
    fft_input[i] = k;     // put real data into even bins
    fft_input[i + 1] = 0; // set odd bins to 0
  }
  
  fft_window();     // window the data for better frequency response
  fft_reorder();    // reorder the data before doing the fft
  fft_run();        // process the data in the fft
  fft_mag_octave(); // take the output of the fft
  sei();

  int ledOut[WIDTH];
  int average = 0;

  for (int i = 0; i < 8; i++)
  {
    ledOut[i + 1] = constrain((fft_oct_out[i] - NOISE_REDUCTION) / 16, 0, 9);
    average = average + ledOut[i];
    Serial.print(fft_oct_out[i]);
    Serial.print(" ");
  }

  average = average / 8;

  ledOut[0] = average;
  ledOut[WIDTH - 1] = average;

  Serial.println();
  // in order to get the nth led of the zth column:
  // x = (Number of Rows * z) + n;
  for (int i = 0; i < WIDTH; i++) {
    for (int j = i * HEIGHT; j < (i * HEIGHT) + ledOut[i]; j++) {
      strip.setPixelColor(j, redOut, blueOut, greenOut);
    }
  }
  strip.show();
  delay(DELAY);
}
