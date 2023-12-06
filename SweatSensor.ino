#include "rgb_lcd.h"
rgb_lcd lcd;

uint8_t buttonPin = 6;
uint8_t motorPin = 4;
uint8_t moisturePin = 5;
uint8_t buttonHold = 0;
uint8_t prevPress = LOW;
uint8_t currPress;
bool State = LOW;

int PWMChannel = 0;
int PWMFreq = 5000;
int PWMResolution = 10;
int MAX_DUTY_CYCLE = 1023;
int dutyCycle;


void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(moisturePin, INPUT);
  ledcSetup(PWMChannel, PWMFreq, PWMResolution);
  ledcAttachPin(motorPin, PWMChannel);
  lcd.begin(16, 1);
}
float moisturePercent(uint8_t moisturePin) {
  uint32_t max = 8000;
  uint32_t min = 2500;
  uint32_t rawValue = analogRead(moisturePin);

  float percent = 100.0 * (max - rawValue) / (max - min);

  return percent;
}
void loop() {
  currPress = digitalRead(buttonPin);
  if (currPress != prevPress) {
    if (currPress == HIGH) {
      State = !State;
    }
    prevPress = currPress;
  }
  if (State == HIGH) {
    if (moisturePercent(moisturePin) <= 25) {
      lcd.clear();
      lcd.print("Start Moving!");
      lcd.setCursor(1, 1);
      delay(300);
      ledcWrite(PWMChannel, dutyCycle);
      dutyCycle = 0;
    }
    if (moisturePercent(moisturePin) >= 25 && moisturePercent(moisturePin) < 50) {
      lcd.clear();
      lcd.print("You are sweating!");
      lcd.setCursor(1, 1);
      delay(300);
      ledcWrite(PWMChannel, dutyCycle);
      dutyCycle = 341;
    }
    if (moisturePercent(moisturePin) >= 50 && moisturePercent(moisturePin) < 70) {
      lcd.clear();
      lcd.print("Take a break!");
      lcd.setCursor(1, 1);
      delay(300);
      ledcWrite(PWMChannel, dutyCycle);
      dutyCycle = 682;
    }
    if (moisturePercent(moisturePin) >= 70) {
      lcd.clear();
      lcd.print("Are you okay?");
      lcd.setCursor(1, 1);
      delay(300);
      ledcWrite(PWMChannel, dutyCycle);
      dutyCycle = 1023;
    }
  } else {
    ledcWrite(PWMChannel, dutyCycle);
    dutyCycle = 0;
  }
  Serial.println(State);
  Serial.println(moisturePercent(moisturePin));
  Serial.println(dutyCycle);
  delay(1000);
}
