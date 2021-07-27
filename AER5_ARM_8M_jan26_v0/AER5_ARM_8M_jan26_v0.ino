#include <EEPROM.h>

int InPin25 = A2;    // pushbutton connected to digital pin 25
int val = 0;      // variable to store the read value

void setup() {
  DDRD = 0xFF; // PORT D OUT RELAY Port
  PORTD = 0x00; // PORT D all Relay are OFF
  DDRC = 0x00; // PORT C IN SWITCH Port
  PORTC = 0x0F; // GATE is PULL-UP ON
  
  //PORTD = 0b00010000; DOWN
  //PORTD = 0b00000100; UP
  pinMode(InPin25, INPUT_PULLUP);    // sets the digital pin 7 as input
  cli(); // No interrupts
}

void loop() {
 
  delay(1000); //0,1 sec
  
  float dataF = 0;
  EEPROM.get(6, dataF);
  if (dataF == 0)
  {
    float dataF = 1;
    EEPROM.put(6, dataF);
    PORTD = 0b00010000;
  }
  else 
  {
    float dataF = 0;
    EEPROM.put(6, dataF);
    PORTD = 0b00000100;
  }

for (int i = 0; i < 200; i++) // first 2 sec
  {
    delay(1000);    
  }
  
  for (int i = 0; i < 900; i++) // timeout 9 sec
  {
    delay(1000);    
    val = digitalRead(InPin25);
    if (val == 0)
    {
      break;
    }
    else {}
  }
loop1:
  PORTD = 0x00;
  goto loop1;

}
