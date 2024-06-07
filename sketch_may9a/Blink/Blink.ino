const int buzzer_Pin = 8;
char sleep_status = 0;

void setup() {
  Serial.begin(9600);
  pinMode(buzzer_Pin, OUTPUT);
}

void loop() {
  while (Serial.available() > 0) {
    sleep_status = Serial.read();
    if (sleep_status == 'a') { 
      digitalWrite(buzzer_Pin, HIGH);
      delay(2000);
      digitalWrite(buzzer_Pin, LOW);
      delay(100);
    } else if (sleep_status == 'b') {
      delay(2000);
    } else {
      /* Do Nothing */
    }
  }
}
