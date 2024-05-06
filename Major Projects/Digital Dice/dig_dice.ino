#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Set the LCD address to 0x27 for a 20 chars and 4 line display

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {22, 23, 24, 25}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {26, 27, 28, 29}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

enum State
{
  WELCOME,
  MENU,
  NUM_ROLLS,
  ROLLING
};

State currentState = WELCOME;
int selectedOption = 0;
int numRolls = 0;
int rolls[20];
int dieType = 0;

void setup() {
  lcd.init(); // initialize the lcd 
  lcd.backlight(); // Turn on the backlight
}

void loop() {
  char key = keypad.getKey();
    switch (currentState)
    {
    case WELCOME:
        lcd.setCursor(4, 1);
        lcd.print("Welcome to");
        lcd.setCursor(3, 2);
        lcd.print("Digital Dice!");
        delay(2000);
        lcd.clear();
        currentState = MENU;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("1. D4 ");
        lcd.setCursor(0, 1);
        lcd.print("2. D6 ");
        lcd.setCursor(0, 2);
        lcd.print("3. D8 ");
        lcd.setCursor(0, 3);
        lcd.print("4. D10");
        lcd.setCursor(9, 1);
        lcd.print("5. D12");
        lcd.setCursor(9, 2);
        lcd.print("6. D20");  
        break;
    case MENU:
        if (key >= '1' && key <= '6')
        {
            selectedOption = key - '0';
            currentState = NUM_ROLLS;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Enter num rolls:");
        }
        break;
    case NUM_ROLLS:
        if (key && key >= '0' && key <= '9')
        {
            numRolls = numRolls * 10 + (key - '0');
            lcd.setCursor(0, 1);
            lcd.print(numRolls);
        }
        else if (key == '#')
        {
            currentState = ROLLING;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Rolling...");
        }
        break;
    case ROLLING:
        lcd.clear();
        if (selectedOption == 1)
        {
          dieType = 4;
        }
        else if (selectedOption == 2)
        {
          dieType = 6;
        }
        else if (selectedOption == 3)
        {
          dieType = 8;
        }
        else if (selectedOption == 4)
        {
          dieType = 10;
        }
        else if (selectedOption == 5)
        {
          dieType = 12;
        }
        else if (selectedOption == 6)
        {
          dieType = 20;
        }
        lcd.setCursor(0, 1);
        lcd.print("Rolls: [");
        for (int i = 0; i < numRolls; i++)
        {
            rolls[i] = random(1, dieType+1); // Generate random roll
            lcd.print(rolls[i]);
            if (i < numRolls - 1)
            {
                lcd.print(", ");
            }
        }
        lcd.print("]");
        delay(2000);
        lcd.clear();
        lcd.setCursor(4, 1);
        lcd.print("Going Back");
        lcd.setCursor(4, 2);
        lcd.print("to Welcome");
        delay(2000);
        lcd.clear();
        currentState = WELCOME; // Go back to the welcome state after rolling
        break;
    }
}
