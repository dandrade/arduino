int led = 11; //pwm
int potencia; 
int ptnFinal; // potencia final

// para leer los datos el potenciometro la pata de en medio va en el analogo para leer los datos que se meten, negativo a GND, positivo a 5v
void setup() {

  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  potencia = analogRead(A0); // potenciometro, dividimos entre 4 para que maximo de 255, que es el maximo voltaje numericamente representado (5v)
  ptnFinal = map(potencia, 0, 1023, 0, 255); // aqui nos omitimos la division, ponemos la variable a mapear, el valor minimo, maximo, y de ahi el valor que representamos
  // ptnFinal = map(potencia, 0, 1023, 10, 50);
  
  Serial.println(ptnFinal);
  analogWrite(led, ptnFinal); // porque estamos usando un puerto pwn, el digital solo enviaba 0, 1 y aca podemos en el analogo ponerle valor
}
