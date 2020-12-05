int led = 13; // pin 13 de arduino

void setup() {

    Serial.begin(9600);
    pinMode(led, OUTPUT); // aqui se pone el pin donde esta conectado el led, como de salida, xq solo vamos a emitir.
}

void loop() {

  digitalWrite(led, LOW); //digitalwrite xq estamos en un pin digital mandamos 0volts para apagarlo.

  delay(3000);

  digitalWrite(led, HIGH); // le manda 5v para encender

  delay(3000);
  
}
