#include "NXP/iolpc2124.h"

// Function to read ADC value from pin 27 (assumed to be AD0.2)
uint16_t ReadADC(void) {
    // Configure and start ADC conversion
    ADCR = (1 << 2) |    // Select channel 2 (AD0.2)
           (4 << 8)  |    // CLKDIV = 4 (adjust based on clock frequency)
           (1 << 21) |    // Power up ADC
           (1 << 24);     // Start conversion now

    // Wait for conversion to complete
    while (!(ADGDR & (1 << 31))); // Check DONE bit

    // Extract and return result (10-bit value)
    return (ADGDR >> 6) & 0x3FF;
}

int main(void) {
    // Configure GPIO pins 0-7 as outputs
    IO0DIR = 0xFF;

    while(1) {
        // Read ADC value
        uint16_t adc_value = ReadADC();
        
        // Convert 10-bit value to 8-bit (scale for LEDs)
        uint8_t led_pattern = (adc_value >> 2); // Simple scaling
        
        // Update LEDs
        IOPIN0 = (IOPIN0 & ~0xFF) | (led_pattern & 0xFF);

        // Simple delay
        for(volatile int i = 0; i < 100000; i++);
    }
}