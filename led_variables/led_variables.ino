String nombre = "Daniel";
int edad = 33;

void setup() {
  Serial.begin(9600); // Velocidad del monitor serial, las mas comun es 9600

}

void loop() {
 Serial.println(nombre);
 delay(1000);  // 1mil milisegundos = 1 segundo
 Serial.println(edad);
 delay(1000);

}
