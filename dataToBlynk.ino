
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "____"
#define BLYNK_TEMPLATE_NAME "____"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"


float voltage;
int bat_percentage;
int analogInPin  = A0;    // Analog input pin
int sensorValue;
float calibration = 0.36; // Check Battery voltage using multimeter & add/subtract the value



void setup()
{
  Serial.begin(115200);
  delay(100);

  BlynkEdgent.begin();

}

void loop() {
  BlynkEdgent.run();
 

  sensorValue = analogRead(analogInPin);
  voltage = (((sensorValue * 3.3) / 1024) * 2 + calibration); //multiply by two as voltage divider network is 100K & 100K Resistor

  bat_percentage = mapfloat(voltage, 2.8, 4.2, 0, 100); //2.8V as Battery Cut off Voltage & 4.2V as Maximum Voltage

  if (bat_percentage >= 100)
  {
    bat_percentage = 100;
  }
  if (bat_percentage <= 0)
  {
    bat_percentage = 1;
  }
  
      //send data to blynk
   
    Blynk.virtualWrite(V0, voltage);  // for battery voltage
    Blynk.virtualWrite(V1, bat_percentage);  // for battery percentage
  
    //Print data on serial monitor
  

  Serial.print("Analog Value = ");
  Serial.println(sensorValue);
  Serial.print("Output Voltage = ");
  Serial.println(voltage);
  Serial.print("Battery Percentage = ");
  Serial.println(bat_percentage);

  Serial.println();
  Serial.println("");
  Serial.println();
  delay(1000);
 
       if (bat_percentage <=30)
    {
      Serial.println("Battery level below 30%, Charge battery on time");
      //send notification
      Blynk.logEvent("battery_low", "Battery is getting low.... Plugin to charge") ;
      delay(500);
    }
    if (bat_percentage >=90)
    {
      Serial.println("Battery level above 90%, disCharge battery on time");
      //send notification
      Blynk.logEvent("battery_high", "Battery is getting high.... Plugout  charge") ;
      delay(500);
    }

  // display temperature on OLED
  

  // display humidity on OLED
  
}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


/*This is the refence code from :-
  -----------------------
  ESP8266 Monitor its Own Battery Level using IoT
  -----------------------
  Website: https://iotprojectsideas.com/
*/
