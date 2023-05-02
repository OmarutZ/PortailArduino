#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define LEDPIN 13       
#define SENSORPIN 10

const byte ROWS = 4; 
const byte COLS = 3; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

LiquidCrystal_I2C lcd(0x27, 16, 2);  

const String instruct = "Entrez taille en cm puis appuyez sur #     ";
int length = instruct.length();
String inputString;
int inputInt;

void setup() {
  
  pinMode(LEDPIN, OUTPUT);        // Initalise la LED de la pin 13 comme sortie .  
  pinMode(SENSORPIN, INPUT);      // Initialise le capteur de la pin 10 comme entrée.
  digitalWrite(SENSORPIN, HIGH);  // Allume le capteur.

  lcd.backlight();                // Allume l'éclairage de l'écran.
  lcd.init();                     // Initialise l'écran
  
  Serial.begin(9600);

}

void loop(){

  // Affiche le menu principal.
  lcd.setCursor(0,0);
  lcd.print("1. Instructions");
  lcd.setCursor(0,1);
  lcd.print("2. Commencer");
  
  // Lit l'entrée du clavier.
  char keyMenu = customKeypad.getKey();

  // Affiche les instructions.
  if (keyMenu == '1') {
    lcd.clear();
    for (int i = 0; i<length-15; i++) {
      lcd.setCursor(0,0);
      lcd.print(instruct.substring(i,i+15));
      delay(800);
    }
  }

  // Commence le programme.
  if (keyMenu == '2') {
    long int firsttime = millis();
    long int secondtime = firsttime;
    lcd.clear();

    // Accorde une minute pour le calcul.
    while ((secondtime - firsttime) <=60000) {
      lcd.setCursor(0, 0);
      lcd.print("Taille en cm:");
      secondtime = millis();

      // Lit l'entrée du clavier.
      char keyTaille = customKeypad.getKey();

      if (keyTaille >= '0' && keyTaille <= '9') {     // Vérifie que un chiffre est entré.
        inputString += keyTaille;                     // Ajoute le chiffre entré comme un caractère      
        lcd.setCursor(0, 1);
        lcd.print(inputString);        
      } 
      else if (keyTaille == '#') {                    // Vérifie que # a été appuyé
        if (inputString.length() > 0) {
          long int thirdtime = millis();
          long int fourthtime = thirdtime;
          inputInt = inputString.toInt();             // Change la chaine de caractères en Int.
          inputString = "";                           // Réinitialise la chaine de caractères.  
          lcd.setCursor(13, 0);
          lcd.print(inputInt);
          digitalWrite(LEDPIN, HIGH);                 // Allume la LED pour indiquer l'activité.
          
           while ((fourthtime - thirdtime) <=60000) {
            fourthtime = millis();

            while(digitalRead(SENSORPIN) == HIGH) {
              digitalWrite(LEDPIN, HIGH);
              long int startTime = micros();

              while(digitalRead(SENSORPIN) == LOW) {
                digitalWrite(LEDPIN, LOW);
                long int endTime = micros();
                long int totalTime = endTime - startTime;
                float timeSec = float(totalTime) / 1000000;
                float vitesse = (inputInt / timeSec) / 27.77;
                lcd.clear();
                lcd.setCursor(0, 0); 
                lcd.print(vitesse,2);
                lcd.setCursor(4,0);
                lcd.print("km/h");
              }
            }
          }
        }
      }
    }
  } 
}