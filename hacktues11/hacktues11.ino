#include <LiquidCrystal.h>

// LCD Keypad Shield Pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define IR1 A1  
#define IR2 A2  
#define IR3 A3  
#define BUTTONS A0  // Analog pin for buttons

float sum = 0.0;
int currency = 0;  // 0 = BGN, 1 = EUR, 2 = USD

// Exchange rates (adjust if needed)
const float BGN_TO_EUR = 0.51;
const float EUR_TO_USD = 1.10;
const float USD_TO_BGN = 1.82;

void setup() {
    Serial.begin(115200);
    pinMode(IR1, INPUT);
    pinMode(IR2, INPUT);
    pinMode(IR3, INPUT);

    lcd.begin(16, 2);  
    lcd.print("Sum: 0.00 BGN");
}

void loop() {
    bool updated = false;  // Track if sum changed

    if (digitalRead(IR1) == LOW) {  
        sum += 0.20;
        Serial.println("INC 20");
        updated = true;
        delay(500);  
    }
    if (digitalRead(IR2) == LOW) {  
        sum += 0.50;
        Serial.println("INC 50");
        updated = true;
        delay(500);  
    }
    if (digitalRead(IR3) == LOW) {  
        sum += 1.00;
        Serial.println("INC 100");
        updated = true;
        delay(500);  
    }

    // If sum changed, update the LCD
    if (updated) {
        updateDisplay();
    }

    // Check for button press
    int button = readButton();
    if (button == 1) {  // Left button
        convertCurrency();
        updateDisplay();
        delay(500);  // Debounce delay
    }
}

// Function to update LCD display
void updateDisplay() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sum: ");
    lcd.setCursor(5, 0);
    
    if (currency == 0) {
        lcd.print(sum, 2);
        lcd.print(" BGN");
    } else if (currency == 1) {
        lcd.print(sum * BGN_TO_EUR, 2);
        lcd.print(" EUR");
    } else if (currency == 2) {
        lcd.print(sum * BGN_TO_EUR * EUR_TO_USD, 2);
        lcd.print(" USD");
    }
}

// Function to convert currency
void convertCurrency() {
    currency = (currency + 1) % 3;  // Cycle through BGN -> EUR -> USD -> BGN
    
}

// Function to read button presses from the LCD Keypad Shield
int readButton() {
    int value = analogRead(BUTTONS);

    if (value > 400 && value <600) return 1;  // LEFT button
    return 0;  // No button pressed
}
