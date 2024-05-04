#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 20, 4); // LCD i2c address

// Servo Setup
Servo horizontal; // horizontal servo
Servo vertical;   // vertical servo

// Global Variables for Servo Control
int servoh = 180; // initial horizontal servo position
int servov = 0;   // initial vertical servo position

// Constants for MPPT Modes
const byte BULK = 0;
const byte ABSORPTION = 1;
const byte FLOAT = 2; // renamed to avoid conflicts

// Rest of your code...
// Pin Definitions
const int ldrlt = A1; // LDR top left - BOTTOM LEFT <--- BDG
const int ldrrt = A2; // LDR top right - BOTTOM RIGHT
const int ldrld = A3; // LDR down left - TOP LEFT
const int ldrrd = A4; // LDR down right - TOP RIGHT
const int ldrmt = A5;
const int button1 = 9;
const int button2 = 10;
const int motorA = 7;
const int motorB = 8;
const int solar_voltage_in = A1;
const int solar_current_in = A0;
const int battery_voltage_in = A2;
const int PWM_out = 10;
const int load_enable = 2;

// MPPT Constants
const float bulk_voltage_max = 14.5;
const float bulk_voltage_min = 13;
const float absorption_voltage = 14.7;
const float float_voltage_max = 13;
const float battery_min_voltage = 10;
const float solar_min_voltage = 19;
const float charging_current = 2.0;
const float absorption_max_current = 2.0;
const float absorption_min_current = 0.1;
const float float_voltage_min = 13.2;
const float float_voltage = 13.4;
const float float_max_current = 0.12;
const unsigned long LCD_refresh_rate = 1000;

// Variables
float bat_voltage = 0;
int pwm_value = 0;
float solar_current = 0;
float current_factor = 0.185; // Value defined by manufacturer ACS712 5A
float solar_voltage = 0;
float solar_power = 0;
String load_status = "OFF";
int pwm_percentage = 0;
unsigned long before_millis = 0;
unsigned long now_millis = 0;
String mode_str = "BULK";
byte mode = 0; // 0: BULK, 1: ABSORPTION, 2: FLOAT

void setup() {
  // Servo Setup
  horizontal.attach(5);
  vertical.attach(6);
  horizontal.write(180);
  vertical.write(0);

  // LCD Setup
  lcd.init();
  lcd.backlight();

  // Pin Modes
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(PWM_out, OUTPUT);
  pinMode(load_enable, OUTPUT);
  digitalWrite(load_enable, LOW);

  // Timer Setup for PWM frequency of 31372.55 Hz
  TCCR1B = TCCR1B & B11111000 | B00000001;

  Serial.begin(9600);

  before_millis = millis();
}

void loop() {
  // Solar Panel Tracking
  solarPanelTracking();

  // MPPT Charging Control
  mpptControl();
}

void solarPanelTracking() {
  int ldrStatus = analogRead(ldrmt);
  if (ldrStatus > 30) {
    int buttonStateA = digitalRead(button1);
    if (buttonStateA == LOW) {
      digitalWrite(motorA, HIGH); // Counter-clockwise
      digitalWrite(motorB, LOW);
    } else {
      digitalWrite(motorA, LOW);
      digitalWrite(motorB, LOW);
    }

    // LDR Readings
    int lt = analogRead(ldrlt);
    int rt = analogRead(ldrrt);
    int ld = analogRead(ldrld);
    int rd = analogRead(ldrrd);
    int dtime = 10;
    int tol = 90;
    int avt = (lt + rt) / 2;
    int avd = (ld + rd) / 2;
    int avl = (lt + ld) / 2;
    int avr = (rt + rd) / 2;
    int dvert = avt - avd;
    int dhoriz = avl - avr;

    // Vertical Servo Control
    if (-1 * tol > dvert || dvert > tol) {
      if (avt > avd) {
        vertical.write(++servov);
        if (servov > 60)
          servov = 60;
      } else if (avt < avd) {
        vertical.write(--servov);
        if (servov < 0)
          servov = 0;
      }
    }

    // Horizontal Servo Control
    if (-1 * tol > dhoriz || dhoriz > tol) {
      if (avl > avr) {
        horizontal.write(--servoh);
        if (servoh < 5)
          servoh = 5;
      } else if (avl < avr) {
        horizontal.write(++servoh);
        if (servoh > 175)
          servoh = 175;
      } else if (avl == avr) {
        delay(10);
      }
    }

    delay(dtime);
  } else {
    for (int pos = horizontal.read(); pos <= 180; pos += 1) {
      horizontal.write(pos);
      delay(15);
    }
    for (int pos2 = vertical.read(); pos2 <= 0; pos2 += 1) {
      vertical.write(pos2);
      delay(15);
    }
    int buttonStateB = digitalRead(button2);
    if (buttonStateB == LOW) {
      digitalWrite(motorA, LOW); // Clockwise
      digitalWrite(motorB, HIGH);
    } else {
      digitalWrite(motorA, LOW);
      digitalWrite(motorB, LOW);
    }
  }
}

void mpptControl() {
  bat_voltage = getBatteryVoltage(15);
  solar_voltage = getSolarVoltage(15);
  solar_current = getSolarCurrent(15);
  solar_power = bat_voltage * solar_current;
  pwm_percentage = map(pwm_value, 0, 255, 0, 100);

  now_millis = millis();
  if (now_millis - before_millis > LCD_refresh_rate) {
    before_millis = now_millis;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(1); // Panel icon
    lcd.print(" ");
    lcd.print(solar_voltage, 2);
    lcd.print("V ");
    lcd.write(0); // Battery icon
    lcd.print(" ");
    lcd.print(bat_voltage, 2);
    lcd.print("V");

    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.print(solar_current, 2);
    lcd.print("A LOAD ");
    lcd.print(load_status);

    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.print(solar_power, 2);
    lcd.print("W PWM ");
    lcd.print(pwm_percentage);
    lcd.print("%");

    lcd.setCursor(0, 3);
    lcd.print(mode_str);
  }

  // Load Management
  if (bat_voltage < battery_min_voltage) {
    digitalWrite(load_enable, LOW);
    load_status = "OFF";
  } else {
    digitalWrite(load_enable, HIGH);
    load_status = "ON";
  }

  // MPPT Control
  if (mode == FLOAT) {
    // FLOAT Mode Implementation
  } else {
    // BULK and ABSORPTION Modes Implementation
  }
}

float getSolarVoltage(int n_samples) {
  float voltage = 0;
  for (int i = 0; i < n_samples; i++) {
    voltage += (analogRead(solar_voltage_in) * (5.0 / 1023.0) * 8.0);
  }
  voltage /= n_samples;
  if (voltage < 0) {
    voltage = 0;
  }
  return voltage;
}

float getBatteryVoltage(int n_samples) {
  float voltage = 0;
  for (int i = 0; i < n_samples; i++) {
    voltage += (analogRead(battery_voltage_in) * (5.0 / 1023.0) * 7.85);
  }
  voltage /= n_samples;
  if (voltage < 0) {
    voltage = 0;
  }
  return voltage;
}

float getSolarCurrent(int n_samples) {
  float Sensor_voltage;
  float current = 0;
  for (int i = 0; i < n_samples; i++) {
    Sensor_voltage = analogRead(solar_current_in) * (5.0 / 1023.0);
    current += (Sensor_voltage - 2.5) / current_factor;
  }
  current /= n_samples;
  if (current < 0) {
    current = 0;
  }
  return current;
}
