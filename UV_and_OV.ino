#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EmonLib.h>

const int relay = 10;  

EnergyMonitor emon;

float current_Volts;
unsigned long printPeriod = 500;
unsigned long previousMillis = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4); 

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(relay, OUTPUT);

  emon.voltage(A0, 234.0, 1.7); 
  Serial.begin(9600);
}

void loop() {
  emon.calcVI(20, 2000);      
  current_Volts = emon.Vrms;   

  if (millis() - previousMillis >= printPeriod) {
    previousMillis = millis();

    lcd.setCursor(0, 0);
    lcd.print("Voltage: ");
    lcd.print(current_Volts, 1);
    lcd.print(" V     ");

    if (current_Volts < 180) {
      lcd.setCursor(0, 1);
      lcd.print("Status: Under Volt ");
      digitalWrite(relay, HIGH);
    } 
    else if (current_Volts > 240) {
      lcd.setCursor(0, 1);
      lcd.print("Status: Over Volt  ");
      digitalWrite(relay, HIGH);
    } 
    else {
      lcd.setCursor(0, 1);
      lcd.print("Status: Normal     ");
      digitalWrite(relay, LOW);
    }

    Serial.print(current_Volts);  
    Serial.print(" ");
    Serial.print(180);            
    Serial.print(" ");
    Serial.print(240);            
    Serial.println(" ");


  }
}
