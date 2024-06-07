#include <Wire.h>  // Include Wire library for I2C
#include <LiquidCrystal_I2C.h>  // Include the LiquidCrystal_I2C library

LiquidCrystal_I2C lcd(0x27, 16, 2); // Change the address (0x27) as per your LCD address

const int buzzer_Pin = 8;
const int led_Green_Pin = 9;  // Đèn xanh
const int led_Red_Pin = 10;   // Đèn đỏ
char sleep_status = 0;
char previous_status = 0;

unsigned long previousMillis = 0;
const long interval = 800;
bool buzzerState = LOW;

void setup() {
  lcd.clear();
  Serial.begin(9600);  
  pinMode(buzzer_Pin, OUTPUT);
  pinMode(led_Green_Pin, OUTPUT);
  pinMode(led_Red_Pin, OUTPUT);
  lcd.init();  // Initialize the LCD
  lcd.backlight();  // Turn on the backlight
  lcd.print("Flying");
  lcd.setCursor(0, 1); // Set cursor to the second row
  lcd.print("Detection SYSTEM");
  digitalWrite(buzzer_Pin, LOW); 
  digitalWrite(led_Green_Pin, LOW);
  digitalWrite(led_Red_Pin, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    sleep_status = Serial.read();
  }

  if (sleep_status != previous_status) {
    previous_status = sleep_status;
    lcd.clear();
    digitalWrite(buzzer_Pin, LOW);
    digitalWrite(led_Green_Pin, LOW);
    digitalWrite(led_Red_Pin, LOW);

    if (sleep_status == 'a') { 
      lcd.print("Bird");
      buzzerState = HIGH;
      digitalWrite(led_Red_Pin, LOW);
      digitalWrite(led_Green_Pin, HIGH);  // Bật đèn xanh
    } 
    else if (sleep_status == 'b' || sleep_status == 'c') {
      lcd.print(sleep_status == 'b' ? "Drone" : "Airplane");
      buzzerState = HIGH;
      digitalWrite(led_Green_Pin, LOW);
      digitalWrite(led_Red_Pin, HIGH);  // Bật đèn đỏ
    } 
    else if (sleep_status == 'd') {
      lcd.print("Stop");
      buzzerState = LOW;
    }

    digitalWrite(buzzer_Pin, buzzerState);
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (buzzerState == HIGH) {
      buzzerState = LOW;
    } else {
      buzzerState = HIGH;
    }

    digitalWrite(buzzer_Pin, buzzerState);
  }
}
