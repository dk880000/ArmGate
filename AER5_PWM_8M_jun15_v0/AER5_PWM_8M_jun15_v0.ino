#include <EEPROM.h>

int Out_PD2 = 2;   // PD2 OUT DAC
int Out_PD3 = 3;   // PD3 OUT DAC
int Out_PD4 = 4;   // PD4 OUT DAC
int Out_PD5 = 5;   // PD5 OUT DAC
int Out_PD6 = 6;   // PD6 OUT DAC
int Out_PD7 = 7;   // PD7 OUT DAC
int In_PB7 = 15;     // PINB 7 Doorbell Digital Input 7
int In_PB6 = 14;     // PINB 6 Fan ON Digital Input 6
int In_PB1 = 9;      // PINB 1 Lamp turn ON Digital Input 1
int In_PB0 = 8;      // PINB 0 Limit SW Digital Input 0
int Out_PC3 = A4; // PINC 3 U
int Out_PC2 = A2; // PINC 2 V
int Out_PC1 = A1; // PINC 1 W
int Out_PC0 = A0; // PINC 0 
//int VBat = A7;       // V Bat sensor ADC 7 in
//int VPV = A6;        // V Solar panel ADC 6 in
int val = 0;      // variable to store the read value
int var = 0;      // variable for different operations
int dc = 60;    // Duty cicle Hz
long MSpeed = 0;   // variable for Motor Speed 0=stop 60=full speed

float dataF = 0;    // direction write/read EEPROM
char data_rcvd = 0; // serial data




void setup() {
  DDRB = 0b00000000; // PORT B IN digital Inputs Port
  PORTB = 0b00000000; // PORT B pull-ups are OFF

  DDRC = 0b00001111; // PORT C OUT Port
  PORTC = 0b00000000; // PORT C, UVW GATEs are OFF

  DDRD = 0b11111100; // PORT D OUT RELAY Port
  PORTD = 0b10000000; // PORT D LAMPs and Motor are OFF

  Serial.begin(9600);       // initialize UART with baud rate of 9600 bps

  cli(); // No interrupts
}

void loop() {
    for (int i = 10000; i > 4100; i = i - 100) // timeout 9 sec
    {
      MSpeed = i;
        for (int pwm = 0; pwm < 5; pwm++)
        {
          if (MSpeed <= 10000)
          { //CW 
              for (long c = MSpeed; c != 0; c--) PORTC = 5; for (long c = MSpeed; c != 0; c--) PORTC = 1; for (long c = MSpeed; c != 0; c--) PORTC = 3; for (long c = MSpeed; c != 0; c--) PORTC = 2; for (long c = MSpeed; c != 0; c--) PORTC = 6; for (long c = MSpeed; c != 0; c--) PORTC = 4;
          }
          else {PORTC = 0;}
        }
    }

       MSpeed = 4000;
         for (int pwm = 0; pwm <= 1000; pwm++)
         { //CW
             for (long c = MSpeed; c != 0; c--) PORTC = 5; for (long c = MSpeed; c != 0; c--) PORTC = 1; for (long c = MSpeed; c != 0; c--) PORTC = 3; for (long c = MSpeed; c != 0; c--) PORTC = 2; for (long c = MSpeed; c != 0; c--) PORTC = 6; for (long c = MSpeed; c != 0; c--) PORTC = 4;
         }

     for (int i = 4000; i < 10000; i = i + 100) // timeout 9 sec
     {
       MSpeed = i;
        for (int pwm = 0; pwm < 5; pwm++)
        {
          if (MSpeed <= 10000)
          { //CW
            for (long c = MSpeed; c != 0; c--) PORTC = 5; for (long c = MSpeed; c != 0; c--) PORTC = 1; for (long c = MSpeed; c != 0; c--) PORTC = 3; for (long c = MSpeed; c != 0; c--) PORTC = 2; for (long c = MSpeed; c != 0; c--) PORTC = 6; for (long c = MSpeed; c != 0; c--) PORTC = 4;
          }
          else {PORTC = 0;}
        }
     }


    for (int i = 10000; i > 4100; i = i - 100) // timeout 9 sec
    {
      MSpeed = i;
        for (int pwm = 0; pwm < 5; pwm++)
        {
          if (MSpeed <= 10000)
          { //CCW
            for (long c = MSpeed; c != 0; c--) PORTC = 1; for (long c = MSpeed; c != 0; c--) PORTC = 5; for (long c = MSpeed; c != 0; c--) PORTC = 4; for (long c = MSpeed; c != 0; c--) PORTC = 6; for (long c = MSpeed; c != 0; c--) PORTC = 2; for (long c = MSpeed; c != 0; c--) PORTC = 3;
          }
          else {PORTC = 0;}
        }
    }

       MSpeed = 4000;
         for (int pwm = 0; pwm <= 1000; pwm++)
         {//CCW
            for (long c = MSpeed; c != 0; c--) PORTC = 1; for (long c = MSpeed; c != 0; c--) PORTC = 5; for (long c = MSpeed; c != 0; c--) PORTC = 4; for (long c = MSpeed; c != 0; c--) PORTC = 6; for (long c = MSpeed; c != 0; c--) PORTC = 2; for (long c = MSpeed; c != 0; c--) PORTC = 3;
         }

     for (int i = 4000; i < 10000; i = i + 100) // timeout 9 sec
     {
       MSpeed = i;
        for (int pwm = 0; pwm < 5; pwm++)
        {
          if (MSpeed <= 10000)
          {//CCW
            for (long c = MSpeed; c != 0; c--) PORTC = 1; for (long c = MSpeed; c != 0; c--) PORTC = 5; for (long c = MSpeed; c != 0; c--) PORTC = 4; for (long c = MSpeed; c != 0; c--) PORTC = 6; for (long c = MSpeed; c != 0; c--) PORTC = 2; for (long c = MSpeed; c != 0; c--) PORTC = 3;
          }
          else {PORTC = 0;}
        }
     }
}
