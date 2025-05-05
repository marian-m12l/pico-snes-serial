#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "joypad.pio.h"

 
#define LATCH_PIN 13
#define CLOCK_PIN 14
#define DATA_PIN 15


// Bits order: B Y Select Start Up Down Left Right / A X L R 0 0 0 0
typedef struct {
    uint16_t b:1;
    uint16_t y:1;
    uint16_t select:1;
    uint16_t start:1;
    uint16_t up:1;
    uint16_t down:1;
    uint16_t left:1;
    uint16_t right:1;
    uint16_t a:1;
    uint16_t x:1;
    uint16_t l:1;
    uint16_t r:1;
    uint16_t signature:4;
} buttons_t;

typedef union {
    buttons_t buttons;
    uint16_t raw;
} input_t;

input_t data = {0};

// Counter to press DOWN button every 60 polls
uint16_t counter = 0;

// TODO Custom ID for serial? 3 nibbles worth of data? one byte only?
// TODO or simply ignore controller ID and use both bytes as data???

PIO pio;
int sm;

static void pio_irq_func(void) {
    // Push current input to FIFO
    pio_sm_put_blocking(pio, sm, data.raw);
    
    // Press DOWN button every 60 polls
    counter++;
    data.buttons.down = (counter % 60) == 0;

    //printf("inputs: 0x%04x down=0x%x\n", data.raw, data.buttons.down);
}

int main() {
    stdio_init_all();

    // Initialize PIO program to answer controller polls from console
    pio = pio0;
    sm = pio_claim_unused_sm(pio, true);
    uint offset = pio_add_program(pio, &joypad_program);
    joypad_program_init(pio, sm, offset, LATCH_PIN);

    // Setup IRQ handler
    int pio_irq = pio_get_irq_num(pio, 0);
    irq_set_exclusive_handler(pio_irq, pio_irq_func);
    irq_set_enabled(pio_irq, true);
    pio_set_irq0_source_enabled(pio, pis_interrupt0 + sm, true);

    // Start PIO program
    pio_sm_set_enabled(pio, sm, true);


    while(true) {
        // Wait for PIO interrupt and push next inputs into FIFO
        //tight_loop_contents();
        sleep_ms(2000);
        printf(".\n");
    }
    return 0;
}
