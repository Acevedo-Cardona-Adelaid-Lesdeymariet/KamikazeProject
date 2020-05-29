int ledPinRojo = 10;
int ledPinVerde = 12;
int pinBuz = 6; 
int ultrasonido = 9;

void setup()
{
    Serial.begin(9600);
    pinMode(ledPinRojo, OUTPUT);
    pinMode(ledPinVerde, OUTPUT);
}

long readUltrasonicDistance(int pin)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin,LOW);
    delayMicroseconds(2);
    digitalWrite(pin,HIGH);
    delayMicroseconds(10);
    digitalWrite(pin, LOW);
    pinMode(pin, INPUT);
    return pulseIn(pin, HIGH);
}

void loop()
{
    int cm = 0.01723*readUltrasonicDistance(ultrasonido);
    if (cm < 200)
    {
        tone(pinBuz, 350, 200);
        digitalWrite(ledPinRojo, HIGH);
        Serial.println("Motion detected!");
        digitalWrite(ledPinVerde, LOW);
    }
    else
    {
        digitalWrite(ledPinRojo, LOW);
        digitalWrite(ledPinVerde, HIGH);
    }
}
