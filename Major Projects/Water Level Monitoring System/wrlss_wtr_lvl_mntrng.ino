#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 

#define TRIG_PIN 11
#define ECHO_PIN 12
#define BUZZER_PIN 10

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Set the LCD address to 0x27 for a 20 chars and 4 line display

const int pinR = 3;
const int pinG = 4;
const int pinB = 5;
const int GREEN_LIMIT_LOW = 30; // Green LED will light up below this limit
const int GREEN_LIMIT_HIGH = 70; // Green LED will light up above this limit
const int RED_LIMIT = 10; // Red LED will light up below this limit
const int BLUE_LIMIT = 90; // Blue LED will light up above this limit

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight(); // Turn on the backlight
  lcd.setCursor(4, 1);
  lcd.print("Welcome to");
  lcd.setCursor(0, 2);
  lcd.print("Water Level Monitor");
  delay(2000);
  lcd.clear();

  // Initialize LED
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
  //Initialize the LCD
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Water Level:");
  // Read water level using ultrasonic sensor
  float distance = getDistance();

  // Display water level on LCD
  lcd.setCursor(0, 1);
  lcd.print("                    "); // Clear previous reading
  lcd.setCursor(0, 1);
  lcd.print(distance);

  // Determine tank status
  if (distance <= RED_LIMIT) {
    lcd.setCursor(0, 2);
    lcd.print("                    "); // Clear previous reading
    lcd.setCursor(0, 2);
    lcd.print("Tank Empty"); // Tank is empty
    analogWrite(pinR, 255);
    analogWrite(pinG, 0);
    analogWrite(pinB, 0);
  } else if (distance >= BLUE_LIMIT) {
    lcd.setCursor(0, 2);
    lcd.print("                    "); // Clear previous reading
    lcd.setCursor(0, 2);
    lcd.print("Tank Overflow"); // Tank is overflowing
    analogWrite(pinR, 0);
    analogWrite(pinG, 0);
    analogWrite(pinB, 255);
  } else if (distance >= GREEN_LIMIT_LOW && distance <= GREEN_LIMIT_HIGH) {
     lcd.setCursor(0, 2);
    lcd.print("                    "); // Clear previous reading
    lcd.setCursor(0, 2);
    lcd.print("Tank Normal"); // Tank is within normal range
    analogWrite(pinR, 0);
    analogWrite(pinG, 255);
    analogWrite(pinB, 0);
  }

  // Sound buzzer if water level is too high or too low
  if (distance < RED_LIMIT || distance > BLUE_LIMIT) {
    tone(BUZZER_PIN, 1000); // Sound buzzer at 1000 Hz
  } else {
    noTone(BUZZER_PIN); // Turn off buzzer
  }

  delay(1000); // Delay for stability
  lcd.clear();
}

float getDistance() {
  // Trigger ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo duration
  unsigned long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance in cm
  float distance = duration * 0.034 / 2;

  return distance;
}
