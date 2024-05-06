#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Define the LCD module settings
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Define the pins for the servo and keypad
#define SERVO_PIN 9
#define ROWS 4
#define COLS 4
#define BUZZER_PIN 10

// Define the password
const char* password = "1234";

// Define the keypad layout
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Define the pins for the keypad
byte rowPins[ROWS] = {22, 23, 24, 25};
byte colPins[COLS] = {26, 27, 28, 29};

// Create the keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Create the servo object
Servo lockServo;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  
  // Start with a welcome message
  lcd.setCursor(4, 1);
  lcd.print("Welcome to");
  lcd.setCursor(2, 2);
  lcd.print("Digital Lock !");
  
  // Initialize the servo
  lockServo.attach(SERVO_PIN);
  
  // Wait for a brief moment before proceeding
  delay(2000);

  // Clear the LCD screen
  lcd.clear();
  
  // Prompt the user to enter the password
  lcd.setCursor(0, 1);
  lcd.print("Enter password:");
}

void loop() {    
  // Get the input from the keypad
  char key = keypad.getKey();
  
  if (key != NO_KEY) {
    // If a key is pressed, add it to the password buffer
    static char inputPassword[5];
    static int index = 0;
    
    if (key == '#') {
      // If the user presses the '#' key, check if the entered password is correct
      if (strcmp(inputPassword, password) == 0) {
        // If the passwords match, unlock the lock
        lcd.clear();
        lcd.setCursor(1, 1);
        lcd.print(" Password Ok !");
        lcd.setCursor(0, 2);
        lcd.print(" Lock Unlocked !");
        unlock();
        // Clear the LCD screen
        lcd.clear();
        // Prompt the user to enter the password
        lcd.setCursor(0, 1);
        lcd.print("Enter password:");
      } else {
        // If the passwords do not match, display an error message
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Error :");
        lcd.setCursor(0, 2);
        lcd.print("Wrong Password !");
        // Play the musical tune
        tone(BUZZER_PIN, 262); // C4
        delay(100); // Delay for 100 milliseconds
        tone(BUZZER_PIN, 294); // D4
        delay(100); 
        tone(BUZZER_PIN, 262); // C4
        delay(100); 
        tone(BUZZER_PIN, 294); // D4
        delay(100); 
        noTone(BUZZER_PIN); // Stop playing the tone
        // Clear the LCD screen
        lcd.clear();
        // Prompt the user to enter the password
        lcd.setCursor(0, 1);
        lcd.print("Enter password:");
      }
      
      // Reset the input buffer
      index = 0;
      memset(inputPassword, 0, sizeof(inputPassword));
    } else if (index < sizeof(inputPassword) - 1) {
      // If the key is not '#' and there is space in the input buffer, add the key to the buffer
      inputPassword[index++] = key;
      lcd.setCursor(index - 1, 2);
      lcd.print('*'); // Mask the password on the LCD
    }
  }
}

void unlock() {
  // Rotate the servo to unlock the lock
  lockServo.write(0); // Adjust the angle as needed
  // Play the musical tune
  tone(BUZZER_PIN, 262); // C4
  delay(100); // Delay for 100 milliseconds
  tone(BUZZER_PIN, 294); // D4
  delay(100);
  tone(BUZZER_PIN, 330); // E4
  delay(100);
  tone(BUZZER_PIN, 349); // F4
  delay(100);
  tone(BUZZER_PIN, 392); // G4
  delay(100);
  tone(BUZZER_PIN, 440); // A4
  delay(100);
  tone(BUZZER_PIN, 494); // B4
  delay(100);
  tone(BUZZER_PIN, 523); // C5
  delay(100);
  noTone(BUZZER_PIN); // Stop playing the tone
  lockServo.write(90); // Return the servo to its initial position
}
