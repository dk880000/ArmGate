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
  //digitalWrite(A0, 0);
  //digitalWrite(A1, 0);
  //digitalWrite(A2, 0);
  //digitalWrite(A3, 0);
  //PORTC = 0b00000001; LIGHT
  //PORTC = 0b00000010; UP
  //PORTC = 0b00000100; DOWN
  //PORTC = 0b00001000; FULL SPEED

  //pinMode(8, INPUT);
  //pinMode(9, INPUT);
  //pinMode(10, INPUT);
  //pinMode(11, INPUT);
  //pinMode(12, INPUT);
  //pinMode(13, INPUT);
  //pinMode(14, INPUT);
  //pinMode(15, INPUT);
  //PortIn =  bitRead(PINB, 0); //stop SW
  //PortIn =  bitRead(PINB, 1); //Light
  //PortIn =  bitRead(PINB, 6); //reserve1
  //PortIn =  bitRead(PINB, 7); //reserve2
  //pinMode(InPinSW, INPUT_PULLUP);    // sets the digital pin  as input
  //pinMode(InPinLIGHT, INPUT_PULLUP);    // sets the digital pin  as input
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
    
  //Delay for 2 sec
    for (int i = 0; i < 200; i++) // first 2 sec
    {
      delay(1000);    
    }
    
  //Turn On Light and Up motor in Full speed
    PORTC = 0b00001011; // UP + LIGHT

  //Main moving 10 sec
    for (int i = 0; i < 1000; i++) // timeout 9 sec
    {
      delay(1000);    

   // If we get Stop command we stop moving
      val = bitRead(PINB,0); //SW STOP
      if (val == 0)
      {
   //Turn On Light and Up motor in Half speed
        PORTC = 0b00000011; // UP + LIGHT 
        break;
      }
      else {}
    }
   //Turn On Light and Up motor in Half speed
    PORTC = 0b00000011; // UP + LIGHT 
  }
///////////////////////////////////////////////  
  else 
  {
    // If Down condition we write Up condition in EEPROM
    float dataF = 0;
    EEPROM.put(6, dataF);

    //Turn On Light and Down motor in Half speed
    PORTC = 0b00000101; //DOWN + LIGNT
    
    //Delay for 2 sec
    for (int i = 0; i < 200; i++) // first 2 sec
    {
      delay(1000);    
    }
    
    //Turn On Light and Down motor in Full speed
    PORTC = 0b00001101; // UP + LIGHT    
        //Main moving 10 sec
    for (int i = 0; i < 1000; i++) // timeout 9 sec
    {
      delay(1000);    

      // If we get Stop command we stop moving
      val = bitRead(PINB,0); //SW STOP
      if (val == 0)
      {
        //Turn On Light and Down motor in Half speed
        PORTC = 0b00000101; //DOWN + LIGNT
        break;
      }
      else {}
    }
    //Turn On Light and Down motor in Half speed
    PORTC = 0b00000101; //DOWN + LIGNT
  }


loop1:
    //Delay for 0,5 sec
    for (int i = 0; i < 50; i++) // first 2 sec
    {
      val = bitRead(PINB,1); //Light
      if (val == 0)
      {
        //Turn On Light and Down motor in Half speed
        PORTC = 0b00000001; //DOWN + LIGNT
        //Delay for 5 sec
        for (int i = 0; i < 500; i++) // first 2 sec
        {
          delay(1000);    
        }
        PORTC = 0b00000000;   
      }
      delay(1000);    
    }
  PORTC = 0b00000000;   
  goto loop1;

}
