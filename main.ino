#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int enA = 10;
const int in1 = 9;
const int in2 = 8;
const int buttonPin = 12;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int buttonState = 0;         
int lastButtonState = 0;     
unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;    

const unsigned long restTime = 5000;
unsigned long motorRunTime = 0;
unsigned long motorStartTime = 0;

int mode = 0;
const char* modeNames[] = {"Big Dog", "Medium Dog", "Small Dog"};
const unsigned long modeRunTimes[] = {5000, 3000, 1000};

void setup() {
  lcd.init();
  lcd.backlight();
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  updateLCD();
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        mode = (mode + 1) % 3;
        updateLCD();
      }
    }
  }

  lastButtonState = reading;

  if (millis() - motorStartTime > modeRunTimes[mode] + restTime) { 
    motorRunTime = modeRunTimes[mode];
    motorStartTime = millis();
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, 128);
  }

  if (millis() - motorStartTime > motorRunTime) {
    analogWrite(enA, 0);
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(modeNames[mode]);
}
