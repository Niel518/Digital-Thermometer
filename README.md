# Digital-Thermometer
A digital thermometer implemented with the DS18B20 programmable 1-wire digital thermometer and the PIC18F4550 MCU.

# DS18B20.c

### Initialization of DS18B20 (DS18B20_init):

Configures RA0 as an output and sends a reset signal to the DS18B20 sensor.
Configures RA0 as an input and checks for the DS18B20 response.
Prints messages based on the success or failure of the DS18B20 reset.

### DS18B20 Reset Function (DS18B20_reset):

Configures RA0 as an output and sends a reset signal to the DS18B20 sensor.
Configures RA0 as an input and checks for the DS18B20 response.

### DS18B20 Write Function (DS18B20_Write):

Writes a byte of data to the DS18B20 sensor.
Uses bit manipulation to send each bit of the data byte to the sensor.
Delays for 90 microseconds after setting RA0 high or low for each bit.

### DS18B20 Read Function (DS18B20_Read):

Reads a byte of data from the DS18B20 sensor.
Uses bit manipulation to extract each bit of data and store it in an array.
Combines the bits to form the byte of data.

## NOT WORKING YET 

### DS18B20 Delay Function (DS18B20_DelayMicroseconds):

Configures Timer0 for a specified delay in microseconds.
Uses a 16-bit mode with the internal instruction cycle clock (Fosc/4).
Disables the prescaler to achieve precise microsecond delays.
Calculates the number of Timer0 overflows needed for the delay.
Enables Timer0 interrupts, waits for the delay to complete, and then disables Timer0 interrupts.

### Timer0 Interrupt Service Routine (Timer0_ISR):

Clears the Timer0 overflow flag.
Increments the flag to indicate the delay is complete.

# TODO LIST
- [ ] Fix the Timer0 so it can count in milliseconds accurately. Right now, it works **only** for the value us=500.
- [ ] Implement the rest of the functions of DS18B20
- [ ] Compile an *actual* program that can measure temperature:)
