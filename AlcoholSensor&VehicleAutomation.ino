#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int sensorPin = A0;         // MQ3 sensor connected to analog pin A0
int sensorValue = 0;        // Variable to store sensor reading
float alcoholLevel = 0;     // Alcohol concentration level (in percentage)
int buzzerPin = 8;          // Buzzer connected to pin 8
int relayPin = 7;           // Relay connected to pin 7
int threshold = 60;        // Alcohol concentration threshold (0 to 100%)

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD I2C address 0x27, size 16x2

void setup() {
  lcd.init();               // Initialize the LCD
  lcd.backlight();          // Turn on backlight
  pinMode(buzzerPin, OUTPUT);  // Set buzzer as output
  pinMode(relayPin, OUTPUT);   // Set relay as output
  digitalWrite(relayPin, HIGH); // Relay OFF initially (motor ON if active LOW)

  Serial.begin(9600);       // Begin serial for debugging

  lcd.setCursor(0, 0);
  lcd.print("Alcohol Detector");
  delay(2000);
}

void loop() {
  sensorValue = analogRead(sensorPin);  // Read analog value (0-1023)

  // Map the sensor reading to a percentage (adjust 280 and 420 based on calibration)
  alcoholLevel = map(sensorValue, 340, 460, 0, 100);

  // ✨ Don't constrain the upper limit so that it can go above threshold!
  if (alcoholLevel < 0) alcoholLevel = 0;

  // Print for debugging
  Serial.print("Sensor: ");
  Serial.print(sensorValue);
  Serial.print("  Alcohol Level: ");
  Serial.print(alcoholLevel);
  Serial.println("%");

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);+
  lcd.print("Alcohol: ");
  lcd.print(alcoholLevel);
  lcd.print("%");

  if (alcoholLevel >= threshold) {
    digitalWrite(buzzerPin, HIGH);   // Buzzer ON
    digitalWrite(relayPin, LOW);     // Relay OFF (Motor OFF if active LOW)
    lcd.setCursor(0, 1);
    lcd.print("ALERT! Too High!");
    delay(2000); // Optional pause for alert visibility
  } else {
    digitalWrite(buzzerPin, LOW);    // Buzzer OFF
    digitalWrite(relayPin, HIGH);    // Relay ON (Motor ON)
    lcd.setCursor(0, 1);
    lcd.print("Level Safe     ");
  }

  delay(1000); // Delay before next reading
}
