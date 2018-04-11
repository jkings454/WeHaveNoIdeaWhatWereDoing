#define LOG_OUT 1 

#include <FFT.h>

const int inPin = A0;

int count = 0;
int val = 0;

void setup() {
    Serial.begin(9600);
}

void loop() {
    int count = 0;
    while (count < 512) {
        val = analogRead(inPin);
    
        fft_input[count] = val;
        fft_input[count++] = 0;
        count++;
    }
    count = 0;

    fft_window();
    fft_reorder();
    fft_run();
    fft_mag_log();

    for (int i = 0; i < 128; i++) {
        Serial.print(fft_log_out[i]);
        Serial.print(" ");
    }
    Serial.println();

    delay(600);
}