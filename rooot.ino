#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Define the analog pins for sensors
const int soilMoisturePin = A0;
const int mq5Pin = A1;  // Assuming the MQ5 gas sensor is connected to A1

// Define I2C address for the LCD
const int lcdAddress = 0x27;
const int lcdCols = 16;
const int lcdRows = 2;

// Create LCD object
LiquidCrystal_I2C lcd(lcdAddress, lcdCols, lcdRows);

// Define the pin for the servo motor
const int servoPin = 9;

// Create Servo object
Servo myServo;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  myServo.attach(servoPin);

  lcd.print("Waste Segregation");
  lcd.setCursor(0, 1);
  lcd.print("System");
  delay(2000);
  lcd.clear();
}

void loop() {
  int soilMoistureValue = analogRead(soilMoisturePin);
  int moistureThreshold = 500;

  int mq5Value = analogRead(mq5Pin);

  // Correct the mapping for moisture percentage
  int moisturePercentage = map(soilMoistureValue, 1023, 0, 0, 100);

  if (soilMoistureValue < moistureThreshold) {
    Serial.println("Wet Waste Detected");
    lcd.clear();
    lcd.print("Wet Waste");
    lcd.setCursor(0, 1);
    lcd.print("Moisture: 100%");
    myServo.write(90);
  } else if (moisturePercentage >= 30) {
    Serial.println("Wet Waste Detected");
    lcd.clear();
    lcd.print("Wet Waste");
    lcd.setCursor(0, 1);
    lcd.print("Moisture: " + String(moisturePercentage) + "%");
    myServo.write(90);
  } else {
    Serial.println("Dry Waste Detected");
    Serial.print("Moisture Percentage: ");
    Serial.println(moisturePercentage);
    lcd.clear();
    lcd.print("Dry Waste");
    lcd.setCursor(0, 1);
    lcd.print("Moisture: " + String(moisturePercentage) + "%");
    myServo.write(0);
  }

  // Display gas sensor values on Serial Monitor
  Serial.print("MQ5 Value: ");
  Serial.println(mq5Value);

  // Display MQ5 sensor reading on LCD for 5 seconds
  lcd.setCursor(0, 1);
  lcd.print("MQ5: " + String(mq5Value));
  delay(5000);
  
  lcd.clear();

  delay(1000);
}
