#include "DS18B20.h"
#include <xc.h>

volatile unsigned int delayFlag; // Flag to indicate when a delay is complete

// Function to initialize DS18B20
void DS18B20_init(void) {
    // Configure RA0 as output and send a reset signal to DS18B20
    TRISAbits.RA0 = 0b0;
    LATAbits.LA0 = 0b0;
    DS18B20_DelayMicroseconds(500);
    
    // Configure RA0 as input and check for DS18B20 response
    TRISAbits.RA0 = 0b1;
    DS18B20_DelayMicroseconds(125);
    
    if (!PORTAbits.RA0) {
        // DS18B20 reset successful
        printf("DS18B20 reset successful.");
    } else {
        // DS18B20 not detected
        printf("Where the fuck is the thermometer?");
    }
}

void DS18B20_reset(void) {
    // Configure RA0 as output and send a reset signal to DS18B20
    TRISAbits.RA0 = 0b0;
    LATAbits.LA0 = 0b0;
    DS18B20_DelayMicroseconds(500);
    
    // Configure RA0 as input and check for DS18B20 response
    TRISAbits.RA0 = 0b1;
    DS18B20_DelayMicroseconds(125);
}

// Function to write a byte of data to DS18B20
void DS18B20_Write(volatile unsigned char data){
    unsigned char i;
    bit smol; // A bit variable to hold each bit of 'data'.
    TRISAbits.RA0 = 0b0; // Set RA0 as output.
    
    for (i=0; i<8; i++){ // Loop through each bit of 'data'.
        smol = (data >> i) & 0x01; // Extract the i-th bit of 'data'.
        if (!smol){ // If the bit is 0...
            LATAbits.LA0 = 0b0; // ...set RA0 low...
            DS18B20_DelayMicroseconds(90); // ...and wait for 90 microseconds.
        }else { // If the bit is 1...
            LATAbits.LA0 = 0b1; // ...set RA0 high...
            DS18B20_DelayMicroseconds(90); // ...and wait for 90 microseconds.
        }
    }
}

// Function to read a byte of data from DS18B20
char DS18B20_Read(void){
    unsigned char i;
    unsigned char bitArray[8]; // Array to store the bits of data
    unsigned char data = 0; // Variable to store the byte of data
    TRISAbits.RA0 = 0b0; // Set RA0 as output
    
    for (i=0; i<8; i++){ // Loop through each bit
        LATAbits.LA0 = 0b0; // Pull the line low
        DS18B20_DelayMicroseconds(5); // Wait for 15 microseconds
        TRISAbits.RA0 = 0b1; // Release the line
        DS18B20_DelayMicroseconds(15); // Wait for 15 microseconds before reading the bit
        bitArray[i] = PORTAbits.RA0; // Read the bit
        data |= bitArray[i] << i; // Shift the bit to its correct position in 'data' and add it to 'data'
    }
    
    return data; // Return the byte of data
}

// Function to create microseconds delay using Timer0
void DS18B20_DelayMicroseconds(unsigned int us) {
    // Configure Timer0 for the delay
    T0CONbits.T08BIT = 0b0; // 16-bit mode
    T0CONbits.T0CS = 0b0;   // Internal instruction cycle clock (Fosc/4)
    T0CONbits.PSA = 0b0;    // Disable prescaler
    T0CONbits.TMR0ON = 0b1; // Timer0 is now ON
    
    // Calculate the number of Timer0 overflows needed for the delay
    unsigned int overflows = us*0.1;

    // Load the initial Timer0 value
    TMR0 = 65536 - overflows;
    
    delayFlag = 0;
   
    // Enable Timer0 interrupts
    INTCONbits.TMR0IF = 0;
    INTCONbits.GIE = 1;
    INTCONbits.TMR0IE = 1;
    
    // Wait for the delay to complete
    while (!delayFlag);
    // Disable Timer0 interrupts
    INTCONbits.TMR0IE = 0;
}

// Timer0 interrupt service routine
void __interrupt(void) Timer0_ISR(void) {
    if (INTCONbits.TMR0IF && INTCONbits.TMR0IE) {
        // Clear Timer0 overflow flag
        INTCONbits.TMR0IF = 0;

        // Increment the flag to indicate the delay is complete
        delayFlag++;
    }
}