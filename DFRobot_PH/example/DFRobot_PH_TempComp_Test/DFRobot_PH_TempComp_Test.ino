

/*
   file DFRobot_PH_TempComp_Test_NTC.ino

   Modified from a modification of the DFRobot_PH library
   
   You can send commands in the serial monitor to execute the calibration.
   Serial Commands (type in serial monitor and hit CTRL+ENTER):
     enterph -> enter the calibration mode
     calph   -> calibrate with the standard buffer solution, two buffer solutions(4.0 and 7.0) will be automaticlly recognized
     exitph  -> save the calibrated parameters and exit from calibration mode

Type 'enterph' then place in a buffer solution then when stabilized type 'calph' then 'exitph' and do it again for the other.
Note that using the USB voltage to calibrate vs the regulated or DC-IN voltage will result in differing values.

   Copyright   GNU Lesser General Public License

   version  V1.2
   date  2022-03
*/

#include "DFRobot_PH.h"
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SmoothThermistor.h>

// pH Probe with amp board available for cheap online many places.  
// VCC->5V, GND->GND, Po->A0

// NTC is generic 10K thermistor waterproof with beta value 3950
// A1 <- 10K resistor -> 5V
// GND <- NTC -> A1
 
#define PH_PIN A0
#define NTC_PIN A1

float voltage, phValue, temperature = 25;

DFRobot_PH ph;
LiquidCrystal_I2C lcd(0x27, 16, 2);
SmoothThermistor smoothThermistor(NTC_PIN);

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("   pH Probe      ");
  lcd.setCursor(0, 1);
  lcd.print(" Initializing... ");
  delay(2000);
  lcd.clear();
  ph.begin();
}

void loop()
{

  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) {                 //time interval: 1s
    timepoint = millis();
    temperature = smoothThermistor.temperature();     
    voltage = analogRead(PH_PIN) / 1024.0 * 5000;     
    phValue = ph.readPH(voltage, temperature);        
    Serial.print("Temperature:");
    Serial.print(temperature, 1);
    Serial.print("C  pH:");
    Serial.println(phValue, 2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("  pH: ");
    lcd.print(phValue);
  }
  ph.calibration(voltage, temperature);               // calibration process by Serial CMD
}
