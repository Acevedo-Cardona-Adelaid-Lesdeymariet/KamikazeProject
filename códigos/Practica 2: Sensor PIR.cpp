int ledR = 10;
int ledV = 12;
int pir = 7;
int buzzer = 6;

void setup(){

  Serial.begin(9600);
  
  pinMode(pir, INPUT_PULLUP);
  pinMode(ledR, OUTPUT);
  pinMode(ledV, OUTPUT);
  
}

void loop(){

  int proximidad = digitalRead(pir);
  delay(100);
  
  if (proximidad == HIGH){
    tone(buzzer,350,200);
    digitalWrite(ledR, HIGH);
    Serial.println("Movimiento Detectado!");
    digitalWrite(ledV, LOW);
  }
  else{
    digitalWrite(ledR, LOW);
    digitalWrite(ledV, HIGH);
  }
}
