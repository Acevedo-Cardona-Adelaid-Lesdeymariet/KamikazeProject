// Código del Arduino que Envía Información

int rx = 0;
int tx = 1;

void setup()
{
  Serial.begin (112500);
  pinMode (rx, OUTPUT);
  pinMode (tx, INPUT);
}

void loop()
{
  byte dataR = 0;
  Serial.print(dataR);
}

// Código del Arduino que Recibe Información

int rx = 0;
int tx = 1;

void setup()
{
  Serial.begin (112500);
  pinMode (rx, OUTPUT);
  pinMode (tx, INPUT);
}

void loop()
{
  if(Serial.available() > 0)
  {
    val = Serial.read();
    if (val == '0')
    {
      // Hace algo.
    }
  }
}
