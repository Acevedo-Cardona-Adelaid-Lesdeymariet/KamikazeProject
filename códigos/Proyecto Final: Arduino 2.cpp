#include <EEPROM.h>

int rx = 0;
int tx = 1;

int buzzer = 5;

int ultrasonido = 8; 
int pirPin = 7;
int pirPin2 = 13;

int ledNaranja = 3;
int ledVerde = 12; 
int ledPinRojoH1 = 9;  
int ledPinRojoH2 = 10;
int ledPinRojoH3 = 11;

byte val; 
byte dataR; 

enum State { NONE, FULL_ALARM, HALF_ALARM, ALARM_SPLIT };
State estado;
long readUltrasonicDistance(int pin);
void loopAlarmaCompleta();
void enciendeMediaAlarma();
void loopMediaAlarma();

void setup()
{ 
  Serial.begin (112500);
  delay(500);
  pinMode(ledNaranja, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  
  pinMode(buzzer, OUTPUT);
  pinMode(ledPinRojoH1, OUTPUT);
  pinMode(ledPinRojoH2, OUTPUT);
  pinMode(ledPinRojoH3, OUTPUT);
  
  pinMode(pirPin, INPUT_PULLUP);
  pinMode(pirPin2, INPUT_PULLUP);
  pinMode(ultrasonido, INPUT);
  
  pinMode (rx, OUTPUT);
  pinMode (tx, INPUT);
  
  estado = NONE;
}

void apagaBuzzer() {
  noTone(buzzer); 
}
void enciendeLedVerde() {
  digitalWrite(ledVerde, HIGH);  
}
void enciendeLedNaranja() {
  digitalWrite(ledNaranja, HIGH);  
}
void apagaLedVerde() {
  digitalWrite(ledVerde, LOW);  
}
void apagaLedNaranja() {
  digitalWrite(ledNaranja, LOW);  
}
void buzzer1() {
 tone(buzzer,350); 
}
void buzzerClaveCorrecta() {
  delay(200); 
  tone(buzzer,500);
  delay(100);
  noTone(buzzer);
  tone(buzzer,600);
  delay(100);
  noTone(buzzer);
  tone(buzzer,800);
  delay(100);
  noTone(buzzer);
}

void buzzerClaveIncorrecta() {
  delay(200); 
  tone(buzzer,300);
  delay(100);
  noTone(buzzer);
  tone(buzzer,100);
  delay(100);
  noTone(buzzer);
}

void buzzerAlarma() {
  tone(buzzer, 800); 
  delay(200);
  tone(buzzer, 500); 
  delay(200);
  noTone(buzzer);
}

bool intrusos = false;
bool alarmOn = false;
bool halfAlarmOn = false;

bool hab1On = false;
bool hab2On = false;
bool hab3On = false;

void desactivaDeteccion() {

  digitalWrite(ledPinRojoH1, LOW);
  digitalWrite(ledPinRojoH2, LOW);
  digitalWrite(ledPinRojoH3, LOW);
  apagaBuzzer();
  enciendeLedVerde();
  apagaLedNaranja();
  alarmOn = false;
  halfAlarmOn = false;

}

void loop()
{
  if(Serial.available() > 0)
  {
     val = Serial.read();  

    /*
    0 - Enciende Led Verde.
    1 - Apaga Led Verde.
    2 - Enciende Led Naranja.
    3 - Apaga Led Naranja.
    4 - Apaga Buzzer.
    5 - Enciende Media Alarma.
    6 - Desactiva Detección.
    7 - Activa Habitación 1.
    8 - Activa Habitación 2.
    9 - Activa Habitación 3.
    10 - Change Pass (Cambio de Contraseña).
    */
    
    if (val == '0') {
      desactivaDeteccion();
    } else if (val == '1') {
      apagaLedVerde();
    } else if (val == '2') {
      enciendeLedNaranja();
      alarmOn = true;
    } else if (val == '3') { 
      buzzerClaveIncorrecta();
    } else if (val == '4') {
      apagaBuzzer();
    } else if (val == '5') {
      enciendeMediaAlarma(); 
    } else if (val == '6') {
      desactivaDeteccion(); 
      buzzerClaveCorrecta();
    } else if (val == '7') {
      hab1On = true;
    } else if (val == '8') {
      hab2On = true;
    } else if (val == '9') {
      hab3On = true;
    }
   
  }  else {
      //buzzerClaveCorrecta();
    }

  if (alarmOn) {
    loopAlarmaCompleta();
  } else if (halfAlarmOn) {
    loopMediaAlarma();
  }
  
  delay(200);   
}

long readUltrasonicDistance(int pin)
{
  pinMode(pin, OUTPUT); 
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin, LOW);
  pinMode(pin, INPUT);
  return pulseIn(pin, HIGH);
}

void loopAlarmaCompleta() 
{
  bool desactivando = false; 
  int proximity = digitalRead(pirPin);
  delay(100);
  
  int proximity2 = digitalRead(pirPin2);
  delay(100);
  
  int cm = 0.01723 * readUltrasonicDistance(ultrasonido);  

  if (proximity == HIGH) 
  {
    digitalWrite(ledPinRojoH1, HIGH);
    intrusos = true;
  }
  else
  {
    digitalWrite(ledPinRojoH1, LOW);
    desactivando = true;
  }
  
  if (proximity2 == HIGH) 
  {
    digitalWrite(ledPinRojoH3, HIGH);
    intrusos = true;
    desactivando = false;
  }
  else
  {
    digitalWrite(ledPinRojoH3, LOW);
  }
  
  if (cm < 200) {
    
    digitalWrite(ledPinRojoH2, HIGH);
    intrusos = true;
  }
  else
  {
    digitalWrite(ledPinRojoH2, LOW);
    if (desactivando == true) {
        intrusos = false; 
    }
  }
  
  if (intrusos) {
    buzzerAlarma();
  }
}

void enciendeMediaAlarma() {
  enciendeLedNaranja();
  apagaLedVerde();
  halfAlarmOn = true;
}

void loopMediaAlarma() 
{
  bool desactivando = false; 

  int proximity = digitalRead(pirPin);
  delay(100);
  
  int cm = 0.01723 * readUltrasonicDistance(ultrasonido);
  
  if (proximity == HIGH) 
  {
    digitalWrite(ledPinRojoH1, HIGH);
    intrusos = true;
  }
  else
  {
    digitalWrite(ledPinRojoH1, LOW);
    desactivando = true;
  }

  if (cm < 200) { 
    digitalWrite(ledPinRojoH2, HIGH);
    intrusos = true;
  }
  else
  {
    digitalWrite(ledPinRojoH2, LOW);
    if (desactivando == true) {
        intrusos = false; 
    }
  }
  
  if (intrusos) {
    buzzerAlarma();
  }
}
