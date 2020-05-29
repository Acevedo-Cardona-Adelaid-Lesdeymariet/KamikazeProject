int ledPinRojo = 8 ;
int ledPinRojo2 = 10;
int ledPinRojo3 = 11;
int ledPinVerde = 12;
int pirPin = 7;
int pirPin2 = 13;
int pinBuz = 6;
int ultrasonido = 9;

void setup()
{
  Serial.begin(9600);
  pinMode(pirPin, INPUT_PULLUP);
  pinMode(pirPin2, INPUT_PULLUP);
  
  pinMode(ultrasonido, INPUT);
  
  pinMode(ledPinRojo,OUTPUT);
  pinMode(ledPinRojo2,OUTPUT);
  pinMode(ledPinRojo3,OUTPUT);
  pinMode(ledPinVerde,OUTPUT);
  
}

long readUltrasonicDistance( int pin)
{
pinMode(pin,OUTPUT);
digitalWrite(pin,LOW);
delayMicroseconds(2);

digitalWrite(pin,HIGH);
delayMicroseconds(10);
digitalWrite(pin,LOW);
pinMode(pin,INPUT);
  
return pulseIn(pin,HIGH);

}

bool intrusos = false;

void loop()
{
  bool desactivado = false;
  
  int proximity = digitalRead(pirPin);
  delay(100);
  
  int proximity2 = digitalRead(pirPin2);
  delay(100);
  
  int cm = 0.01723 * readUltrasonicDistance(ultrasonido);
  Serial.println(cm);
  
if(proximity == HIGH)
  {
  
  tone(pinBuz, 350,200);
  digitalWrite(ledPinRojo, HIGH);
  Serial.println("Motion detected");
  digitalWrite(ledPinVerde,LOW);
  intrusos = true;
  
  }
  else
  {
  digitalWrite(ledPinRojo,LOW);
    desactivado = true;
  }
  
  if(proximity2 == HIGH)
  {
  tone(pinBuz, 350,200);
  digitalWrite(ledPinRojo3, HIGH);
  Serial.println("Motion detected");
  digitalWrite(ledPinVerde,LOW);
  intrusos = true;
  desactivado = false;
  }
  
  else
  {
  digitalWrite(ledPinRojo3,LOW);
  }
    
    if(cm < 200){
      tone(pinBuz, 350,200);
      digitalWrite(ledPinRojo2, HIGH);
      Serial.println("Motion detected");
      digitalWrite(ledPinVerde, LOW);
      intrusos = true;
    }
   else
   {
     digitalWrite(ledPinRojo2 , LOW);
     
     if(desactivado == true) 
     {
     intrusos = false;
     }
   }
  
      if(intrusos == false) 
      {
      digitalWrite(ledPinVerde , HIGH);
      }
      
    }
