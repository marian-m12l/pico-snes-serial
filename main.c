#include <stdio.h>
#include "pico/stdlib.h"

 
#define LATCH_PIN 13
#define CLOCK_PIN 14
#define DATA_PIN 15


// Data line is **active LOW** --> unpressed button is HIGH on data line, 0 on SNES CPU
// Last 4 bits are always 0000 for a standard controller --> HIGH on data line
// Bits order: B Y Select Start Up Down Left Right / A X L R 0 0 0 0
uint16_t data = 0xffff;

// Counter to press DOWN button every 60 polls
uint16_t counter = 0;

// TODO Custom ID for serial? 3 nibbles worth of data? one byte only?
// TODO or simply ignore controller ID and use both bytes as data???

int main() {
    stdio_init_all();

    // Configure GPIOs
    gpio_init(LATCH_PIN);
    gpio_init(CLOCK_PIN);
    gpio_init(DATA_PIN);
    gpio_set_dir(DATA_PIN, GPIO_OUT);

    // Requires external 3.6Kohms pull-up resistors on LATCH and CLOCK pins
    gpio_pull_up(LATCH_PIN);
    gpio_pull_up(CLOCK_PIN);

    // Data line is low at rest
    gpio_put(DATA_PIN, 0);

    while(true) {

        // Wait for latch pulse
        while (gpio_get(LATCH_PIN) == 0) {
            tight_loop_contents();
        }
        while (gpio_get(LATCH_PIN) != 0) {
            tight_loop_contents();
        }
        
        // Output first bit on latch falling edge
        gpio_put(DATA_PIN, (data >> 15) & 0x1);
        
        // Output the next 15 bits
        for (int i=14; i>=0; i--) {
            // Wait for next clock pulse
            while (gpio_get(CLOCK_PIN) != 0) {
                tight_loop_contents();
            }
            while (gpio_get(CLOCK_PIN) == 0) {
                tight_loop_contents();
            }

            // Output next bit on clock rising edge
            gpio_put(DATA_PIN, (data >> i) & 0x1);
        }

        // Wait for the last clock pulse
        while (gpio_get(CLOCK_PIN) != 0) {
            tight_loop_contents();
        }
        while (gpio_get(CLOCK_PIN) == 0) {
            tight_loop_contents();
        }
        
        // Data line is low at rest
        gpio_put(DATA_PIN, 0);

        // Press DOWN button every 60 polls
        counter++;
        if ((counter % 60) == 0) {
            data = 0xfbff;
        } else {
            data = 0xffff;
        }

    }
    return 0;
}
