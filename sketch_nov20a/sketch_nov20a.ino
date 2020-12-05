
const byte zcPin = 12; // D6
const byte pwmPin = 13; // D7 
void setup() {
  // put your setup code here, to run once:
  pinMode(zcPin, INPUT_PULLUP);
  pinMode(pwmPin, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.begin(9600);
  
  for(int i = 10; i <= 255; i+10){
    Serial.println(i);
    digitalWrite(pwmPin, i); 
   }
  
}
