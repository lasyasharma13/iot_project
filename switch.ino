int SwitchPin=0;
int LedPin=16;
int Switchvalue;
void setup() {
  // put your setup code here, to run once:
  pinMode(LedPin,OUTPUT);
  pinMode(SwitchPin,INPUT_PULLUP);
  
}

void loop() {
  // put your main code here, to run repeatedly:
Switchvalue=digitalRead(SwitchPin);
digitalWrite(LedPin,!Switchvalue);
}
