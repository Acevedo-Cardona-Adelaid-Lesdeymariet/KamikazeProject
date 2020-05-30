#include <LiquidCrystal.h>
#include <Keypad.h>

/*---------------------KEYPAD---------------------------*/

const byte numRows=4;
const byte numCols=4;
char keypressed;
char keymap[numRows][numCols]=
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};

byte rowPins[numRows]={7,6,A5,A4};
byte colPins[numCols]={A3,A2,A1,A0};

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

/*--------------------FIN KEYPAD-----------------------*/

/*--------------------LCD------------------------------*/

LiquidCrystal lcd(8,9,10,11,12,13);//Pines (rs,eb,d4, d5, d6,d7)

int screenWidth = 16;
int screenHeight = 2;

/*--------------------FIN LCD-------------------------*/

char codigoSecreto[4] = {'2','2','5','5'};

int posicion=0;
int clave=0;

int cursor=5; 

int ledRojo=2;
int ledVerde=3;
int buzzer=4;

void setup()
{
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  digitalWrite(ledRojo,HIGH);
  digitalWrite(ledVerde,LOW);
  
  lcd.begin(screenWidth, screenHeight);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Introduzca clave");
  lcd.setCursor(cursor,1);
}

void loop()
{
 char pulsacion = myKeypad.getKey();
 if(pulsacion!=0)
 {
   if(pulsacion != '#' && pulsacion != '*' && clave == 0)
   {
     lcd.print(pulsacion);
     cursor++;
     tone(buzzer,350);
     delay(200);
     noTone(buzzer);
     
     if (pulsacion == codigoSecreto[posicion])
     {
       posicion ++;
     }
     if (posicion == 4)
     {
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Clave correcta");
       delay(2000);
       tone(buzzer,500);
       delay(100);
       noTone(buzzer);
       tone(buzzer,600);
       delay(100);
       noTone(buzzer);
       tone(buzzer,800);
       delay(100);
       noTone(buzzer);
       
       clave=1;
       
       digitalWrite(ledRojo,LOW);
       digitalWrite(ledVerde,HIGH);
     }
     if(cursor>8)
     {
       cursor=5;
       posicion=0;
       if(clave==0)
       {
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Clave incorrecta");
         tone(buzzer,70,500);
         delay(250);
         noTone(buzzer);
         delay(1500);
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Introduzca clave");
         lcd.setCursor(5,1);
       }
     }
   }
 }
  if(pulsacion=='*')
  {
    posicion =0;
    cursor=5;
    clave=0;
    posicion=0;
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Introduzca clave");
    lcd.setCursor(5,1);
    
    digitalWrite(ledRojo,HIGH);
    digitalWrite(ledVerde,LOW);
  }
}
