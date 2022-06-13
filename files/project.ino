#include <Stepper.h> // Stepper Library
#include <LiquidCrystal_I2C.h> // LiquidCrystal for I2C Library
#include <WiFi.h> // WiFi Library
#include <HTTPClient.h> // HTTPClient Library
#include <Wire.h> // Wire Library
#include "RTClib.h" // RTClib Library

RTC_DS3231 rtc; // Define RTC
DateTime now; // ESP32 Time

// WiFi
const char* ssid = ""; // Define the SSID for the WiFi
const char* password = ""; // Define the Password for the WiFi
const char* serverName = "ecoseed**.com"; // Define the Server for the data

// Ldr Sensor
const int ldrPin = 4; // LDR Sensor Pin

// Moisture Sensor
int sensor_pin = 34; // Moisture Sensor Pin

// Force Sensor
const int forcePin = 32; // Force Sensor Pin

// Ultrasonic Sensor
const int trigPin = 33; // Trigger Pin
const int echoPin = 25; // Echo Pin
#define SOUND_SPEED 0.034 // Sound Speed

// Leds
const int led = 26; // LED Pin

// Relay
const int relayPin = 2; // Relay Pin

// Lcd I2c
const int lcdColumns = 16; // Define the Number of Columns for the LCD
const int lcdRows = 2; // Define the Number of Rows for the LCD
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); // Create the LCD

// Steppers
#define S1IN1 13 // IN1 for the 1st Stepper
#define S1IN2 12 // IN2 for the 1st Stepper
#define S1IN3 14 // IN3 for the 1st Stepper
#define S1IN4 27 // IN4 for the 1st Stepper

#define S2IN1 18 // IN1 for the 2st Stepper
#define S2IN2 5 // IN2 for the 2st Stepper
#define S2IN3 17 // IN3 for the 2st Stepper
#define S2IN4 16 // IN4 for the 2st Stepper
const int stepsPerRevolution = 2048; // Steps per Revolution
Stepper ldrStepper(stepsPerRevolution, S1IN1, S1IN3, S1IN2, S1IN4); // Create 1st Stepper
Stepper plantStepper(stepsPerRevolution, S2IN1, S2IN3, S2IN2, S2IN4); // Create 2st Stepper

void setup() {
  Serial.begin(115200); // Begin Serial

  lcd.init(); // Start LCD
  lcd.backlight(); // Enable Backlight of LCD

  lcd_welcome(); // Show Welcome Text on LCD

  if (! rtc.begin()) { // Check RTC Module
    Serial.println("Couldn't find RTC");
  }

  // LED Mode
  pinMode(led, OUTPUT);
  // Trigger Mode
  pinMode(trigPin, OUTPUT);
  // Echo Mode
  pinMode(echoPin, INPUT);

  ldrStepper.setSpeed(10); // 1st Stepper Speed
  plantStepper.setSpeed(10); // 2st Stepper Speed

  // Begin Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) { // Check for WiFi status
    delay(500);
    Serial.print(".");
  }

  // Print the IP of the ESP32 to the Console
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Adjust System Time

}

void loop() {
  now = rtc.now(); // Define ESP32 time through RTC

  // Call Functions
  declare_location();
  data_post();
  wifi_led();
  lcd_stats();

  delay(1000); // Wait 1 Second
}
