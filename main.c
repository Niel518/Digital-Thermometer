#include "config.h"
#include "stdio.h"
#include "DS18B20.h"
#include <xc.h>

volatile unsigned int delayFlag1;  // Flag for delay completion

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

// Function to create a delay in microseconds
void delayMicroseconds(unsigned int us) {
    // Configure Timer1 for the delay
    T1CONbits.RD16 = 0b1;
    T1CONbits.T1RUN = 0b0;
    T1CONbits.T1OSCEN = 0b0;
    T1CONbits.TMR1CS = 0b0;
    T1CONbits.T1CKPS = 0b01;
    T1CONbits.TMR1ON = 0b1;

    // Load the initial Timer1 value
    TMR1 = 65536 - us;
    
    delayFlag1 = 0;
   
    // Enable Timer1 interrupts
    PIR1bits.TMR1IF = 0;
    INTCONbits.GIE = 1;
    PIE1bits.TMR1IE = 1;
    
    // Wait for the delay to complete
    while (!delayFlag1);
    // Disable Timer1 interrupts
    PIE1bits.TMR1IE = 0;
}

// Timer1 Interrupt Service Routine
void __interrupt(void) Timer1_ISR(void){
    if (PIR1bits.TMR1IF && PIE1bits.TMR1IE) {
       // Clear Timer0 overflow flag
       PIR1bits.TMR1IF = 0;
       // Increment the flag to indicate the delay is complete
       delayFlag1++;
    }
}

void delayMilliseconds(unsigned int ms){
    int real_ms = ms * 1000;
    int i;
    
    for (i=0; i<real_ms; i++){
        delayMicroseconds(1);
    }
}

// Main function
void main(void){
    setup_osc();              // Configure oscillator settings
    DS18B20_startup();        // Perform DS18B20 startup initialization
    char TMP_LOW;             // Variable to store the low byte of temperature data
    char TMP_HIGH;            // Variable to store the high byte of temperature data
    short TMP;                // Variable to store the combined temperature data
    float TMP_C;              // Variable to store the temperature in Celsius
    
    while(1) {
        DS18B20_Write(RECALL_E2);  // Recall the DS18B20's EEPROM values to the scratchpad
        DS18B20_Write(CONVERT_T);  // Start a temperature conversion
        delayMilliseconds(CONV_TIME_10_BIT);  // Wait for the conversion to complete
        DS18B20_Write(READ_SCRATCHPAD);  // Prepare to read the scratchpad
        TMP_LOW = DS18B20_Read();  // Read the low byte of the temperature data
        TMP_HIGH = DS18B20_Read();  // Read the high byte of the temperature data
        DS18B20_reset();  // Reset the DS18B20
        TMP = TMP_HIGH;  // Store the high byte in TMP
        TMP = (TMP<<8)|TMP_LOW;  // Shift TMP left by 8 bits and OR with the low byte
        TMP = TMP&~3;  // Mask off the lower two bits (since the resolution is 10-bit)
        TMP_C = TMP/16;  // Convert the temperature data to Celsius
        printf("It is %.2f", TMP_C);  // Print the temperature
        printf(" outside");  // Print " outside"
    }
}
