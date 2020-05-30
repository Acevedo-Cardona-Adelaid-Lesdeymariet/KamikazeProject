#include <LiquidCrystal.h>
#include <Keypad.h>

#include <EEPROM.h>

int rx = 0;
int tx = 1;

/*-------------------------------KEYPAD---------------------------------------*/

const byte numRows= 4; 
const byte numCols= 4;
char keypressed;
char keymap[numRows][numCols]=
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'},
};

byte rowPins[numRows] = {7,6,5,4};
byte colPins[numCols] = {A0,A1,A2,A3};            

Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

/*------------------------ FIN DEL KEYPAD---------------------------------------*/

LiquidCrystal lcd(12, 11, 10, 9, 8, 13);

int screenWidth = 16;
int screenHeight = 2;

String lineOne, lineTwo;

int stringStart, stringStop, displayMode, i = 0;
int scrollCursor = screenWidth;

String linea1 = "1 Set alarm";
String linea2 = "2 Set 1/2 alarm";
String linea3 = "3 Change passwd";
String linea4 = "4 Manual config";
String linea1Act = "Alarm set";
String linea2Act = "Enter passwd";
bool alarmSet = false;
bool startAlarm = true;

enum State { NONE, ALARM_SET, CHOSEN_ONE, CHOSEN_TWO, CHOSEN_THREE, CHOSEN_FOUR };
State estado;

void changePass();

void setup() {

  lcd.begin(screenWidth, screenHeight);
  
  Serial.begin (112500);  

  pinMode (rx, OUTPUT);
  pinMode (tx, INPUT);
  
  lcd.clear();
  lcd.setCursor(0,0);     
  estado = NONE;
}

char codigoSecreto[4] = {'2','2','5','5'}; 
int cursor = 5;
int clave=0;
int posicion=0;

void changePass() {
  bool wrong = false;
  int num1, num2, num3, num4;
  int cont = 0;
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("New passwd:");
  lcd.setCursor(1,1);
  lcd.print(">>> ");
  
  while(cont < 4) {
    keypressed = myKeypad.getKey(); 

    if (keypressed != 0) 
    { 
      codigoSecreto[cont] = keypressed;
      lcd.print(keypressed);
      cont++;
    }
  } 
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Passwd updated.");
  delay(2000);
  
  EEPROM.write(0, (int)codigoSecreto[0]);
  EEPROM.write(1, (int)codigoSecreto[1]);
  EEPROM.write(2, (int)codigoSecreto[2]);
  EEPROM.write(3, (int)codigoSecreto[3]);
}

void compruebaNumYActua(int num) {  
  if (keypressed == '1') {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarm set!");
    estado = CHOSEN_ONE;
    
    alarmSet = true;
    startAlarm = true;
    
    byte dataR = 1; 
    Serial.print(dataR);
    delay(300);
    dataR = 2;
    Serial.print(dataR);
    delay(300);
    
  } else if (keypressed == '2') {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Half alarm set!"); 
    estado = CHOSEN_TWO;
    
    alarmSet = true;
    startAlarm = true;
    
    byte dataR = 5;
    Serial.print(dataR);
    delay(100);
    
  } else if (keypressed == '3') {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Changing pass!");
    changePass();
    delay(100);
    estado = CHOSEN_THREE;
    
  } else if (keypressed == '4') {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("You pressed 4!");
    estado = CHOSEN_FOUR;
  }
}

void clearBuffer() {

 byte w = 0;

 for (int i = 0; i < 10; i++)
 {
   while (Serial.available() > 0)
   {
     char k = Serial.read();
     w++;
     delay(1);
   }
   delay(1);
 } 
}

void setupTeclado() 
{
  cursor = 5;
  clave = 0;
  posicion = 0;
  lcd.begin(16,2);      
  lcd.setCursor(0,0);     
  lcd.print("Introduzca clave"); 
  lcd.setCursor(cursor,1); 
}

void loopTeclado() 
{  
  char pulsacion = myKeypad.getKey() ; 
  int numPulsaciones = 0;
  if (pulsacion != 0) 
  { 
    if (pulsacion != '#' && pulsacion != '*' && clave==0)
    { 
      lcd.print(pulsacion); 
      numPulsaciones++;
      cursor++;  
      delay(200);
      
      if (pulsacion == codigoSecreto[posicion]){
        posicion ++; 
      }
      
      if (posicion == 4)
      { 
       
        byte dataX = 0; 
        Serial.print(dataX);
        delay(300);
        
        lcd.setCursor(0,0);      
        lcd.print("Clave correcta  ");         
        delay(300);
        
        dataX = 6;
        Serial.print(dataX);
        delay(300);
        
        alarmSet = false; 
       
        clave=1; 
           
        dataX = 0;
        Serial.print(dataX);
        delay(300);
      } 

      if (numPulsaciones >=4 && posicion != 4) {
         delay(200);
        byte dataR = 3; 
         Serial.print(dataR);
         delay(200);
      }

     if(cursor>8)        
     {  
       cursor=5;     
       posicion=0;           
       lcd.setCursor(5,1);
       lcd.print("    ");       
       lcd.setCursor(5,1);
       if(clave==0)         
       { 
         delay(100);
         byte dataR = 3; 
         Serial.print(dataR);
         delay(100);
         
       }
     }
    }
  } 

 if (pulsacion == '*')
 { 
   posicion = 0;
   cursor = 5;
   clave = 0;
   posicion = 0;
   lcd.setCursor(0,0); 
   lcd.print("Introduzca clave"); 
   lcd.setCursor(5,1);
   lcd.print("    "); 
   lcd.setCursor(5,1);
 }
}

void loop() {
  
  int val1, val2, val3, val4;
  
  val1 = EEPROM.read(0);
  val2 = EEPROM.read(1);
  val3 = EEPROM.read(2);
  val4 = EEPROM.read(3);
  
  if (val1 == 0 && val2 == 0 && val3 == 0 && val4 == 0) {
   
    EEPROM.write(0, (int)codigoSecreto[0]);
    EEPROM.write(1, (int)codigoSecreto[1]);
    EEPROM.write(2, (int)codigoSecreto[2]);
    EEPROM.write(3, (int)codigoSecreto[3]);
  }
  
  val1 = EEPROM.read(0);
  val2 = EEPROM.read(1);
  val3 = EEPROM.read(2);
  val4 = EEPROM.read(3);
  
  if (!alarmSet) {
    setup();
    
    // 1. Activar Alarma. 
    // 2. Activar Alarma (Media Alarma).
    // 3. Cambiar Contraseña.
    // 4. Configuración Manual (Activar/Desactivar Aparatos de uno en uno).

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(linea1);
    lcd.setCursor(0, 1);
    lcd.print(linea2);
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(linea2);
    lcd.setCursor(0, 1);
    lcd.print(linea3);
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(linea3);
    lcd.setCursor(0, 1);
    lcd.print(linea4);
    delay(1000);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Chosen value: "); 
    lcd.setCursor(0,1);
    lcd.print(">>> ");

    bool wrong = false;
    while(true) {
      keypressed = myKeypad.getKey();   

      if (keypressed != 0) 
      { 
        if (keypressed != '#' && keypressed != '*' && keypressed <= '4') {   
          lcd.print(keypressed); 
          break;
        } 
        if (keypressed > '4') {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Wrong value!");
          lcd.setCursor(0,1);
          lcd.print(keypressed);
          lcd.setCursor(1,1);
          lcd.print(" not valid :(");
          wrong = true;
          break;
        }  
      } 
    }

    delay(200);

    if (!wrong) {
      compruebaNumYActua(keypressed);
    } 
    delay(200); 
  }
  else {
    if (startAlarm == true) {
      setupTeclado();
      startAlarm = false;
    }
    loopTeclado();
    if (alarmSet == false) {
      delay(200); 
    }
  }
}
