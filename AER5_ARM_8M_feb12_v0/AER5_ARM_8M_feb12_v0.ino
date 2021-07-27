#include <EEPROM.h>

byte InPinSW = 8;    // pushbutton connected to digital pin 12
byte InPinLIGHT = 9;    // lIGHT connected to digital pin 13
byte val = 0;      // variable to store the read value
//byte PortIn = 0;

void setup() {
  DDRC = 0b00001111;
  PORTC = 0b00000000;
  DDRB = 0;
  DDRD = 0;
  cli(); // No interrupts
}

void loop() {
  //Start delay for contact bounce
  delay(1000); //0,1 sec
  delay(1000); //0,1 sec
  
  //Up or Down condition check in EEPROM 
  float dataF = 0;
  EEPROM.get(6, dataF);
  ///////////////////////////////////////////////////////////
  if (dataF == 0)
  {
  // If Up condition we write Down condition in EEPROM
    float dataF = 1;
    EEPROM.put(6, dataF);

  //Turn On Light and Up motor in Half speed
    PORTC = 0b00000011; // UP + LIGHT 
  //Delay for 1.5 sec
    for (int i = 0; i < 150; i++) // first 2 sec
    {
      delay(1000);    
    }
    val = bitRead(PINB,0); //SW STOP
      if (val == 0)
      {
        goto loop1;
      }
      else {}
    
  //Turn On Light and Up motor in Full speed
    PORTC = 0b00001011; // UP + LIGHT
  //Delay for 2.5 sec
    for (int i = 0; i < 250; i++) // first 2 sec
    {
      delay(1000);    
    }

  //Turn On Light and Up motor in Half speed
    PORTC = 0b00000011; // UP + LIGHT 
  //Delay for 0.5 sec
    for (int i = 0; i < 50; i++) // first 2 sec
    {
      delay(1000);    
    }
    
  //Main moving 7 sec
    for (int i = 0; i < 700; i++) // timeout 9 sec
    {
      delay(1000);    

   //If we get Stop command we stop moving
      val = bitRead(PINB,0); //SW STOP
      if (val == 0)
      {
         break;
      }
      else {}
    }
  }
///////////////////////////////////////////////  
  else 
  {
  //If Down condition we write Up condition in EEPROM
    float dataF = 0;
    EEPROM.put(6, dataF);

  //Turn On Light and Down motor in Half speed
    PORTC = 0b00000101; //DOWN + LIGNT
  //Delay for 1.5 sec
    for (int i = 0; i < 150; i++) // first 2 sec
    {
      delay(1000);    
    }
    val = bitRead(PINB,0); //SW STOP
      if (val == 0)
      {
        goto loop1;
      }
      else {}
    
  //Turn On Light and Down motor in Full speed
    PORTC = 0b00001101; // UP + LIGHT    
  //Delay for 2.5 sec
    for (int i = 0; i < 250; i++) // first 2 sec
    {
      delay(1000);    
    }

  //Turn On Light and Down motor in Half speed
    PORTC = 0b00000101; //DOWN + LIGNT
  //Delay for 0.5 sec
    for (int i = 0; i < 50; i++) // first 2 sec
    {
      delay(1000);    
    }
    
  //Main moving 7 sec
    for (int i = 0; i < 700; i++) // timeout 9 sec
    {
      delay(1000);    

      // If we get Stop command we stop moving
      val = bitRead(PINB,0); //SW STOP
      if (val == 0)
      {
        break;
      }
      else {}
    }
  }


loop1:
      PORTC = 0b00000000;   
      val = bitRead(PINB,1); //Light
      if (val == 0)
      {
        //Turn On Light and Down motor in Half speed
        PORTC = 0b00000001; //DOWN + LIGNT
        //Delay for 5 sec
        for (int i = 0; i < 500; i++) // 5 sec
        {
          delay(1000);    
        }
      }
      delay(1000);    
   
      PORTC = 0b00000000;   
  goto loop1;

}
