#define LOG_OUT 1

#include <FFT.h>

void loop() {
  while(1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei();    
    
    // We have usable FFT data in fft_output, let's convert it into something useful.
    // 10 rows of 0-9 output.

    // 0-25 is our bass.
    int[] freqArr = new int[10];

    for (int i = 0; i < 10, i++) {
      for (int j = i * 25, i < (i * 25) + 25, i++) {
        freqArr[i] = freqArr[i] + fft_output[i];
      }
    }
  }
}