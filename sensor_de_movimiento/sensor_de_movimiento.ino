int pir = 12;
int valor;
int buzz = 7;

void setup() {

    Serial.begin(9600);
    pinMode(pir, INPUT);
    pinMode(buzz, OUTPUT);

}

void loop() {

    valor = digitalRead(pir);
    Serial.println(valor);

    if(valor == 1){
      digitalWrite(buzz, HIGH); 
    }
    else{
      digitalWrite(buzz, LOW);  
    }

}
