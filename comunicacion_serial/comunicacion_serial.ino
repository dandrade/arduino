int led = 13;
String estado; // almacenara la orden

void setup() {

  Serial.begin(9600);
  pinMode(led, OUTPUT);

}

void loop() {

  /*if(estado == "prende"){
    digitalWrite(led, HIGH); 
  }
  else {
    digitalWrite(led, LOW); 
  }*/

  if(Serial.available()){
    estado = Serial.readString(); // Serial.parseInt
    Serial.println(estado); // imprime lo que esta escribiendo
    
    if(estado == "prende"
    || estado == "activa"
    || estado == "prende el led"){
      digitalWrite(led, HIGH);
      Serial.println("El led fue encendido"); 
    }
    
    if(estado == "apaga"
    || estado == "bye"
    || estado == "desactiva led"){
      digitalWrite(led, LOW);
      Serial.println("El led fue apagado"); 
    }
    
  }
}
