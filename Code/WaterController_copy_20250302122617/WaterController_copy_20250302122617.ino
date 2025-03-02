#include <LiquidCrystal.h>
#include <EEPROM.h>
LiquidCrystal lcd(2,3,4,5,6,7);

unsigned long startTime = 0;
unsigned long stopTime = 0;
unsigned long totalTime = 0;
const int eepromAddress = 0;
unsigned long storedTime;
bool timerRunning = false;
const int startbuttonpin = 8;
const int stopbuttonpin = 9;
const int transistor1 = A0;
const int transistor2 = A1;
const int comparator1 = 8;
const int comparator2 = 9;

void setup() {
  pinMode(transistor1,INPUT);
  pinMode(transistor2,INPUT);
  pinMode(comparator1,INPUT);
  pinMode(comparator2,INPUT);
  lcd.begin(16,2);

  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  EEPROM.get(eepromAddress,storedTime);
  Serial.begin(96000);
  Serial.print("Stored Time:");
  Serial.println(storedTime);
  Serial.println("s");
}

void loop() {
  int tran1 =analogRead(transistor1);
  Serial.print(tran1);Serial.print("\t");
  int tran2 =analogRead(transistor2);
  Serial.println(tran2);
  int comp1 = digitalRead(comparator1);
  int comp2 = digitalRead(comparator2);
  if(comp1 == LOW)
  {
    lcd.setCursor(0,2);
    lcd.print("Heator on");
  }
  else
  {
     lcd.setCursor(0,2);
    lcd.print("Heator off");
  }
  if(comp1 == HIGH && comp2 == LOW && !timerRunning)
  {
    startTime = millis();
    timerRunning =true;
    lcd.clear();
    lcd.print("Motor on");
    Serial.println("Motor on");
  }
  else if(comp1 == LOW && comp2 == HIGH && timerRunning)
  {
    stopTime = millis();
    timerRunning =false;
    unsigned long elapsedTime =stopTime -startTime;
    totalTime += elapsedTime;
    lcd.clear();
    lcd.print("Total Time:");
    lcd.print(totalTime / 1000);
    lcd.print("s");
    EEPROM.put(eepromAddress,totalTime);
    Serial.print("Total time:");
    Serial.print(totalTime/1000);
    Serial.print("s");
    Serial.println("Motor off");
  }
  delay(500);
}
