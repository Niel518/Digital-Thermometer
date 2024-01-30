#include "config.h"
#include "stdio.h"
#include "DS18B20.h"
#include <xc.h>

volatile unsigned int delayFlag1;

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
    INTCONbits.TMR0IE = 1;
    
    // Wait for the delay to complete
    while (!delayFlag1);
    // Disable Timer1 interrupts
    PIE1bits.TMR1IE = 0;
}

void __interrupt(void) Timer1_ISR(void){
    if (PIR1bits.TMR1IF && PIE1bits.TMR1IE) {
       // Clear Timer0 overflow flag
       PIR1bits.TMR1IF = 0;
       // Increment the flag to indicate the delay is complete
       delayFlag1++;
    }
}

// Main function
void main(void){
    setup_osc();              // Configure oscillator settings
    DS18B20_startup();        // Perform DS18B20 startup initialization
    char TMP_LOW;
    char TMP_HIGH;
    short TMP;
    float TMP_C;
    
    while(1) {
        DS18B20_Write(RECALL_E2);
        DS18B20_Write(CONVERT_T);
        delayMicroseconds(CONV_TIME_10_BIT);
        DS18B20_Write(READ_SCRATCHPAD);
        TMP_LOW = DS18B20_Read();
        TMP_HIGH = DS18B20_Read();
        DS18B20_reset();
        TMP = TMP_HIGH;
        TMP = (TMP<<8)|TMP_LOW;
        TMP = TMP&~3;
        TMP_C = TMP/16;
        printf("It is 0.2%f", TMP_C);
        printf(" outside");
    }
}
