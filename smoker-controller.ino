#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 11

//create instance of LiquidCrystal_I2C
LiquidCrystal_I2C lcd(0x3F,16,2);

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

float setTemp;
float currentTemp;
boolean setRelay;
int Relay  = 5; // Digital pin D5

void setup(){

    setTemp = 0; 
    currentTemp = 0;
    setRelay = false;
    lcd.begin();
    lcd.backlight();

    lcd.setCursor(0,0);
    lcd.print("Smoker");
    lcd.setCursor(0,1);
    lcd.print("Controller");

    delay(3000);
    lcd.clear();

    Serial.begin(9600);

    sensors.begin();

    lcd.setCursor(0,0);
    lcd.print("Current Temp:");
    pinMode(Relay, OUTPUT);
    digitalWrite(Relay, HIGH);

}

void loop(){
    
    
    sensors.requestTemperatures(); // Send the command to get temperatures

    // After getting the temperatures, print them here.
    // use the function ByIndex to get the temperature from the first device (index 0).
    currentTemp = sensors.getTempFByIndex(0);
    Serial.println(currentTemp); 
    lcd.setCursor(0,1);
    lcd.print(String(currentTemp) + " F");
    if(Serial.available() > 0){
        byte data[4];
        int byteCount = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Received");
        float receivedSetTemp = Serial.parseFloat();
        
        lcd.setCursor(0,1);
        lcd.print(receivedSetTemp);
        setTemp = receivedSetTemp;
        delay(3000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Current Temp:");
    }
    determineFanStatus();
    delay(1000);
}

void determineFanStatus(){
    if((currentTemp < (setTemp - 10)) && !setRelay){
        setRelay = true;
        digitalWrite(Relay, LOW);
    }
    if((currentTemp >= setTemp) && setRelay){
        setRelay = false;
        digitalWrite(Relay, HIGH);
    }
}


