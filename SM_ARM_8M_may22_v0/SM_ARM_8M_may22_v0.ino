#include <EEPROM.h>

int Out_MDir = 2;    // PD2 Out pin Motor Direction 0 = up / 1 = down
int Out_MPWM = 3;    // PD3 Out pin Motor PWM 0 = stop / 1 = max speed
int Out_LAMP1 = 4;   // PD4 OUT LAMP1 Main Light
int Out_LAMP2 = 6;   // PD6 OUT LAMP2 Fan On
int Out_LAMP3 = 7;   // PD7 OUT LAMP3 Battery faul
int In_PB7 = 15;     // PINB 7 Doorbell Digital Input 7
int In_PB6 = 14;     // PINB 6 Fan ON Digital Input 6
int In_PB2 = 10;     // PINB 2 Power Battery faul 0 if Battery V < 10V / 1 if Battery V > 10V
int In_PB1 = 9;      // PINB 1 Lamp turn ON Digital Input 1
int In_PB0 = 8;      // PINB 0 Limit SW Digital Input 0
int In_IRsens2 = A2; // PINC 2 Input IR sensor 2 Object in The Gate Line
int In_IRsens1 = A1; // PINC 1 Input IR sensor 1 Object in front of Gate
int In_IRsens0 = A0; // PINC 0 Input IR sensor 0 Limit SW 
//int VBat = A7;       // V Bat sensor ADC 7 in
//int VPV = A6;        // V Solar panel ADC 6 in
int val = 0;      // variable to store the read value
int var = 0;      // variable for different operations
int dc = 25;    // Duty cicle 
int MSpeed = 0;   // variable for Motor Speed 0=stop 25=full speed

float dataF = 0;    // direction write/read EEPROM
char data_rcvd = 0; // serial data

void setup() {
  DDRB = 0b00000000; // PORT B IN digital Inputs Port
  PORTB = 0b00000000; // PORT B pull-ups are OFF

  DDRC = 0b00111000; // PORT C IN/OUT Port
  PORTC = 0b00000000; // GATE is PULL-UP ON

  DDRD = 0b11111100; // PORT D OUT RELAY Port
  PORTD = 0b10000000; // PORT D LAMPs and Motor are OFF

  Serial.begin(9600);       // initialize UART with baud rate of 9600 bps

  cli(); // No interrupts
}

void loop() {
RESET:
  //data_rcvd = Serial.read();   // read one byte from serial buffer and save to data_rcvd
  
  for (;;){ //Power Battery check
  data_rcvd = PORTD;   Serial.write(data_rcvd); //setd to serial PORTD state
  delay(10000); //0,01 sec
  val = bitRead(PINB, 2); //Power Battery faul
  if (val == 0) PORTD = 0b11000000;
  else   break;  }
      
  //Up or Down condition check in EEPROM
  EEPROM.get(6, dataF);
  /////////////////////////////////////////////////////////// UP //////
  if (dataF == 0)
  { // If Up condition we write Down condition in EEPROM
    dataF = 1;
    EEPROM.put(6, dataF);

    //Slow start of Motor
    PORTD = 0b01011000;    data_rcvd = PORTD;   Serial.write(data_rcvd); //setd to serial PORTD state
    for (int i = 0; i < 24; i++) // timeout 9 sec
    {
      MSpeed = i;
      for (int pwm = 0; pwm < 3000; pwm++){       for (int c = 0; c < dc; c++) { if (c <= MSpeed)  PORTD = 0b01011000; else PORTD = 0b01010000;}    }
    }
            
    //Main moving 10 sec
    for (int i = 0; i < 175; i++) // timeout 9 sec
    {
      if (MSpeed == 0) break;
      else {}
      MSpeed = ceil(25 - i / 7);
      PORTD = 0b01011000;     data_rcvd = PORTD;   Serial.write(data_rcvd); //setd to serial PORTD state
      for (int pwm = 0; pwm < 1000; pwm++){       for (int c = 0; c < dc; c++) { if (c <= MSpeed)  PORTD = 0b01011000; else PORTD = 0b01010000;}    }

      // If we get Stop command we stop moving
      val = bitRead(PINB, 0); //SW STOP
      var = bitRead(PINC, 0); //SW STOP IR
      val = val * var;
      if (val == 0)
      {
        //Turn On Light and Up motor STOP
        for (int ii = 0; ii < 25; ii++)
        {
          if (MSpeed != 0)
          {
            MSpeed--;
            for (int pwm = 0; pwm < 1000; pwm++){       for (int c = 0; c < dc; c++) { if (c <= MSpeed)  PORTD = 0b01011000; else PORTD = 0b01010000;}    }
          }
          else
          {
            if (dataF == 1)         PORTD = 0b00000100; // UP + LIGHT
            else                    PORTD = 0b00000000; // UP + LIGHT
            data_rcvd = PORTD;   Serial.write(data_rcvd); //setd to serial PORTD state
            break;
          }
        }     
      }
      else {}
    }
        
    //Turn OFF Light and Down motor in STOP
    //if (dataF == 1)         PORTD = 0b00000100; // UP + LIGHT
    //else                    PORTD = 0b00000000; // UP + LIGHT
    //data_rcvd = PORTD;   Serial.write(data_rcvd); //setd to serial PORTD state
  }
  /////////////////////////////////////////////// DOWN //////
  else
  {
    // If Down condition we write Up condition in EEPROM
    dataF = 0;
    EEPROM.put(6, dataF);

    //Slow start of Motor
    PORTD = 0b01011100;    data_rcvd = PORTD;   Serial.write(data_rcvd); //setd to serial PORTD state
    for (int i = 0; i < 24; i++) // timeout 9 sec
    {
    MSpeed = i;
    for (int pwm = 0; pwm < 3000; pwm++){       for (int c = 0; c < dc; c++) { if (c <= MSpeed)  PORTD = 0b01011100; else PORTD = 0b01010100;}    }
    val = bitRead(PINC, 2); //IR Object in the gate line
    if (val == 0) goto RESET; //If any object in the gate line Arm goes to UP
    else {}
    }
             
    //Main moving 10 sec
    for (int i = 0; i < 175; i++) // timeout 9 sec
    {
      if (MSpeed == 0) break;
      else {}
      MSpeed = ceil(25 - i / 7);
      PORTD = 0b01011100;  data_rcvd = PORTD;   Serial.write(data_rcvd); //setd to serial PORTD state
      for (int pwm = 0; pwm < 1000; pwm++){       for (int c = 0; c < dc; c++) { if (c <= MSpeed)  PORTD = 0b01011100; else PORTD = 0b01010100;}    }

      // If we get Stop command we stop moving
      val = bitRead(PINC, 2); //IR Object in the gate line
      if (val == 0) goto RESET; //If any object in the gate line Arm goes to UP
      val = bitRead(PINB, 0); //SW STOP
      var = bitRead(PINC, 0); //SW STOP IR
      val = val * var;
      if (val == 0)
      {
        //Turn On Light and Up motor STOP
        for (int ii = 0; ii < 25; ii++)
        {
          if (MSpeed != 0)
          {
            MSpeed--;
            for (int pwm = 0; pwm < 1000; pwm++){       for (int c = 0; c < dc; c++) { if (c <= MSpeed)  PORTD = 0b01011100; else PORTD = 0b01010100;}    }
          }
          else
          {
            if (dataF == 1)         PORTD = 0b00000100; // UP + LIGHT
            else                    PORTD = 0b00000000; // UP + LIGHT
            data_rcvd = PORTD;   Serial.write(data_rcvd); //setd to serial PORTD state
            break;
          }
        }     
      }
      else {}
    }
    
    //Turn OFF Light and Down motor in STOP
    //if (dataF == 1)         PORTD = 0b00000100; // UP + LIGHT
    //else                    PORTD = 0b00000000; // UP + LIGHT
    //data_rcvd = PORTD;   Serial.write(data_rcvd); //setd to serial PORTD state
  }


  for (;;)
  {
      val = bitRead(PINB, 1); //Light
      if (val == 0)
      { if (dataF == 1)         PORTD = 0b01010100; // UP + LIGHT
        else                    PORTD = 0b01010000; // UP + LIGHT 
        data_rcvd = PORTD;   Serial.write(data_rcvd); //setd to serial PORTD state
        for (int i = 0; i < 50; i++) delay(10000);  //Delay for 5 sec
        if (dataF == 1)         PORTD = 0b00000100; // UP + LIGHT
        else                    PORTD = 0b00000000; // UP + LIGHT
        data_rcvd = PORTD;   Serial.write(data_rcvd); //setd to serial PORTD state
      }

      val = bitRead(PINB, 6); //FAN
      if (val == 0)
      { if (dataF == 1)         PORTD = 0b01000100; // UP + LIGHT
        else                    PORTD = 0b01000000; // UP + LIGHT
        data_rcvd = PORTD;   Serial.write(data_rcvd); //setd to serial PORTD state
        for (int i = 0; i < 50; i++) { delay(10000);  //Delay for 5 sec
          val = bitRead(PINB, 1); //Light
          if (val == 0)
          {if (dataF == 1)      PORTD = 0b01010100; // UP + LIGHT
           else                 PORTD = 0b01010000; // UP + LIGHT 
           data_rcvd = PORTD;   Serial.write(data_rcvd); //setd to serial PORTD state
           for (int i = 0; i < 50; i++) delay(10000);  //Delay for 5 sec
          }
          else {}
        }  
        if (dataF == 1)         PORTD = 0b00000100; // UP + LIGHT
        else                    PORTD = 0b00000000; // UP + LIGHT
        data_rcvd = PORTD;   Serial.write(data_rcvd); //setd to serial PORTD state
      }

    delay(3000);
    if (dataF == 1)         PORTD = 0b00000100; // UP + LIGHT
    else                    PORTD = 0b00000000; // UP + LIGHT
    data_rcvd = PORTD;   Serial.write(data_rcvd); //setd to serial PORTD state
    delay(3000);
  }

}
