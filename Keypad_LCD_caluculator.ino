#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD (I2C address, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad configuration
const int ROW_NUM = 4; // Four rows
const int COLUMN_NUM = 4; // Four columns

// Keypad layout
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Pins for rows and columns
byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; // Row pin connections
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; // Column pin connections

// Initialize Keypad
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// Variables for input and operation
String num1 = "";  // First number
String num2 = "";  // Second number
char operation = '\0';  // Operator (+, -, *, /)
bool isSecondNum = false;  // Flag for switching to second number input
bool decimalAdded = false; // Track if decimal point is already added
bool resultDisplayed = false; // Track if result is displayed

void setup() {
  lcd.init();  // Initialize the LCD
  lcd.backlight();  // Turn on backlight
  lcd.setCursor(0, 0);
  lcd.print("Enter Calculation");
  delay(1000);
  lcd.clear();  // Clear the display
}

void loop() {
  char key = keypad.getKey();  // Get the keypress from the keypad

  if (key) {
    // If result was displayed, clear the LCD when typing the next input
    if (resultDisplayed && (key >= '0' && key <= '9')) {
      clearAll();
      lcd.clear();
      resultDisplayed = false;
    }

    // Handle number input (0-9) and decimal point
    if ((key >= '0' && key <= '9') || (key == '*' && !decimalAdded)) {
      if (key == '*') {
        key = '.';  // Replace '*' with actual decimal point '.'
        decimalAdded = true;  // Set flag once decimal is added
      }

      if (!isSecondNum) {
        num1 += key;  // Append the digit or decimal point to num1
        lcd.setCursor(num1.length() - 1, 0);
        lcd.print(key);  // Show num1 on LCD
      } else {
        num2 += key;  // Append the digit or decimal point to num2
        lcd.setCursor(num1.length() + 1 + num2.length(), 0); // Display second number
        lcd.print(key);  // Show num2 on LCD
      }
    }

    // Handle operation input (+, -, *, /)
    if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
      if (key == 'A') {
        operation = '+';
      } else if (key == 'B') {
        operation = '-';
      } else if (key == 'C') {
        operation = '*';
      } else if (key == 'D') {
        operation = '/';
      }
      lcd.setCursor(num1.length(), 0);
      lcd.print(operation);  // Show operator on LCD
      isSecondNum = true;  // Switch to second number input
      decimalAdded = false;  // Reset decimal flag for second number
    }

    // If '#' is pressed, calculate and display the result
    if (key == '#') {
      performCalculation();  // Perform the calculation
      resultDisplayed = true;  // Set flag indicating result is shown
      delay(3000);  // Wait for 3 seconds to let the user see the result
      lcd.clear();  // Clear the display after showing the result
    }

    // If '*' is pressed, clear the inputs
    if (key == '*') {
      clearAll();  // Clear all inputs
      lcd.clear();  // Clear LCD
      lcd.setCursor(0, 0);
      lcd.print("Cleared");
      delay(1000);
      lcd.clear(); 
    }
  }
}

// Function to perform the calculation and display the result
void performCalculation() {
  float number1 = num1.toFloat();  // Convert num1 to a float
  float number2 = num2.toFloat();  // Convert num2 to a float
  float result = 0;

  // Perform the operation
  if (operation == '+') {
    result = number1 + number2;
  } else if (operation == '-') {
    result = number1 - number2;
  } else if (operation == '*') {
    result = number1 * number2;
  } else if (operation == '/') {
    if (number2 != 0) {
      result = number1 / number2;
    } else {
      lcd.setCursor(0, 1);
      lcd.print("Error: Div by 0");
      return;
    }
  }

  // Display the result
  lcd.setCursor(0, 1);
  lcd.print("Result: ");
  lcd.print(result, 2);  // Show the result with 2 decimal places
}

// Function to clear all inputs and reset flags
void clearAll() {
  num1 = "";  // Clear first number
  num2 = "";  // Clear second number
  operation = '\0';  // Clear operator
  isSecondNum = false;  // Reset to first number input
  decimalAdded = false;  // Reset decimal flag
}
