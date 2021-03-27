 #define ONBOARD_LED  2
 
void setupBoardLED() {
  
  Serial.print("setupBoardLED...");
  
  pinMode(ONBOARD_LED,OUTPUT);
  
  boardLED_OFF();
  
  Serial.println("OK");
}

void boardLED_ON()
{
  digitalWrite(ONBOARD_LED,HIGH);
}

void boardLED_OFF()
{
  digitalWrite(ONBOARD_LED,LOW);
}

void boardLED_Blink(int times) {
  for(int i=0;i<times;i++)
  {
  delay(1000);
  digitalWrite(ONBOARD_LED,HIGH);
  delay(100);
  digitalWrite(ONBOARD_LED,LOW);
  }
}
