#include "config.h"
#include "stdio.h"
#include "DS18B20.h"
#include <xc.h>

// Function to configure oscillator settings
void setup_osc(void){
    OSCCONbits.SCS = 0b10;   // Set the system clock to use the internal oscillator
    OSCCONbits.IRCF = 0b111;  // Set internal oscillator to 8 MHz
    OSCCONbits.OSTS = 0;      // Internal oscillator system clock
    OSCCONbits.IDLEN = 1;     // Enable idle mode
}

// Function to perform DS18B20 startup initialization
void DS18B20_startup(){
    DS18B20_init();           // Initialize DS18B20
    DS18B20_Write(SKIP_ROM);  // Skip ROM command
    DS18B20_Write(WRITE_SCRATCHPAD); // Write Scratchpad command
    DS18B20_Write(0x00);      // Placeholder for TH register
    DS18B20_Write(0x00);      // Placeholder for TL register
    DS18B20_Write(RES_10);    // Set resolution to 10-bit
    DS18B20_Write(COPY_SCRATCHPAD); // Copy Scratchpad to EEPROM command
    DS18B20_reset();          // Perform a hardware reset
}

// Main function
void main(void){
    setup_osc();              // Configure oscillator settings
    DS18B20_startup();        // Perform DS18B20 startup initialization
    
    // No specific tasks in the main loop for now
    
    while(1) {
        // Your main program loop, if needed
    }
}