#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define DEBUG 0

#define RELAY_PIN 12
#define RELAY_PIN2 13
bool boiler_heating_on = false;

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 14

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels



// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Addresses of 2 DS18B20s
uint8_t boiler[8] = { 0x28, 0xBA, 0xD4, 0x48, 0xF6, 0x39, 0x3C, 0xB5 };
uint8_t solar[8] = {0x28, 0x2E, 0x38, 0xD4, 0x4D, 0x20, 0x01, 0xB0}; //solar


inline void DisplayTemp(float boiler, float solar);
inline void ControlLogic(float boiler, float solar);
inline void UpdateSensors(float &t_boiler, float &t_solar);

void setup()   
{                
	// initialize with the I2C addr 0x3C
  delay(100);
  sensors.begin();
  delay(100); 

  //display init
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000);
  Serial.println("Display started");  

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);

  // Display static text
  display.println("Startuji");
  display.display();

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); 
  digitalWrite(RELAY_PIN2, HIGH); 

  delay(1000);
}

void loop() {
  float t_boiler;
  float t_solar;

  UpdateSensors(t_boiler, t_solar); //get temps
  ControlLogic(t_boiler, t_solar); //set output
  DisplayTemp(t_boiler, t_solar); //show measured data on display

  delay(5000);   // no need for faster updates
}

inline void UpdateSensors(float &t_boiler, float &t_solar){
  sensors.requestTemperatures();
  t_boiler = sensors.getTempC(boiler);
  t_solar = sensors.getTempC(solar);
}

inline void ControlLogic(float boiler, float solar){
   
  float t_def = abs(boiler - 50.0) * 0.25;
  t_def = t_def > 5.0 ? 5.0 : t_def;
  t_def = t_def < 1.0 ? 1.0 : t_def;
  
  if(boiler_heating_on)
  {
    if(solar<boiler) 
    {
      digitalWrite(RELAY_PIN, HIGH); 
      boiler_heating_on = false;
    }
  }
  else 
  {
    if(solar >= boiler + t_def) 
    {
      digitalWrite(RELAY_PIN, LOW); 
      boiler_heating_on = true;
    }    
  }
}

void DisplayTemp(float boiler, float solar){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("B: ");
  display.println(String(boiler));
  display.setCursor(0, 30);
  display.print("S: ");
  display.println(String(solar));
  display.setTextSize(1);
  display.setCursor(0, 50);
  display.println(boiler_heating_on?"Zapnuto":"Vypnuto");
  display.display(); 
}
