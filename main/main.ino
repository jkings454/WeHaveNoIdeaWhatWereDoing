#define OCTAVE 1
#include <FFT.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, 6, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(115200); // use the serial port
  strip.begin();
  TIMSK0 = 0;    // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40;  // use adc0
  DIDR0 = 0x01;  // turn off the digital input for adc0
}

void loop()
{
  for (int i = 0; i < 8; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
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

  int ledOut[8];
  int average = 0;

  for (int i = 0; i < 8; i++)
  {
    ledOut[i] = constrain(fft_oct_out[i] / 20, 0, 9);
    average = average + ledOut[i];
  }

  average = constrain(average / 8, 0, 9);
  for (int i = 0; i < ledOut[0]; i++)
  {
    strip.setPixelColor(i, strip.Color(150, 0, 0));
  }
  Serial.println(ledOut[0]);
  strip.show();
  delay(1000);
}
