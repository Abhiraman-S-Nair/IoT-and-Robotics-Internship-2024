#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define DHT_PIN 2 // Pin connected to DHT22 data pin
#define BUZZER_PIN 3 // Pin connected to the buzzer
#define TRIG_PIN 12 //Pins connected to Ultrasonic Sensor
#define ECHO_PIN 11

LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address of the LCD

void setup() {
  //Set IP and OP pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  //Intialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 1);
  lcd.print("Welcome to");
  lcd.setCursor(1, 2);
  lcd.print("Distance Measurer");
  delay(2000);
}

void loop() {
  lcd.clear();
  lcd.setCursor(7, 0);
  lcd.print("Data");
  // Read water level using ultrasonic sensor
  float dist_cm = getDistance();
  float dist_in = dist_cm * 0.393;

  // Display water level on LCD
  lcd.setCursor(0, 1);
  lcd.print("Distance: "); // Clear previous reading
  lcd.setCursor(10, 2);
  lcd.print(dist_cm);
  lcd.print("cm");
  lcd.setCursor(10, 3);
  lcd.print(dist_in);
  lcd.print("cm");

  // Sound buzzer if water level is too high or too low
  if (dist_cm < 20) {
    lcd.clear();
    lcd.setCursor(1,1);
    lcd.print("Object too Close !");
    tone(BUZZER_PIN, 1000); // Sound buzzer at 1000 Hz
  } else if (dist_cm > 280){
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("Object too Far !");
    tone(BUZZER_PIN, 1000); // Sound buzzer at 1000 Hz
  }else {
    noTone(BUZZER_PIN); // Turn off buzzer
  }
 
  delay(2000); // Update readings every 2 seconds
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
