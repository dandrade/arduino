int foco = 12;
String estado;

void setup() {

  Serial.begin(9600);
  pinMode(foco, OUTPUT);

}

void loop() {

  if(Serial.available()){
    estado = Serial.readString();
    Serial.println(estado);
    if(estado == "prende el foco"){
      digitalWrite(foco, HIGH);
    }

    if(estado == "apaga el foco"){
      digitalWrite(foco, LOW);
    }
    
  }

}
