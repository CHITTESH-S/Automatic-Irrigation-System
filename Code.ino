// Blynk IOT Smart Agriculture Monitoring System

#define BLYNK_TEMPLATE_ID "TMPL2gXZnCN7N"
#define BLYNK_TEMPLATE_NAME "IoT Smart Agriculture"

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>  // Including library for dht

char auth[] = "tFlsjHk5H3_3Eh7h5unsRrNPD4rnJFjx";       //Authentication code sent by Blynk
char ssid[] = "CIT-campus";                        //WiFi SSID
char pass[] = "citchennai";                //WiFi Password

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN D4          //pin where the dht11 is connected
DHT dht(DHTPIN, DHT22);
#define ONE_WIRE_BUS D6
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define buzzer D5 //Buzzer Pin D5 
#define rainPin D7
int rainState = 0;
int lastRainState = 0;
const int AirValue = 590;   //you need to replace this value with Value_1
const int WaterValue = 300;  //you need to replace this value with Value_2
const int SensorPin = A0;
int soilMoistureValue = 0;
int soilmoisturepercent = 0;
int relay = D0;


#define pirPin D3
int pirValue;
int pinValue;

//Read value from blynk
BLYNK_WRITE(V0)
{
  pinValue = param.asInt();
}



void setup()
{
  Serial.begin(115200);
  delay(100);
  Blynk.begin(auth, ssid, pass);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay();
  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  sensors.begin(); // Dallas temperature
  dht.begin();
}

void getPirValue(void)        //Get PIR Data
{
  pirValue = digitalRead(pirPin);
  if (pirValue)
  {
    Serial.println("Motion detected");
    Blynk.logEvent("Motion detected in your farm") ;
  }
}

void loop() {
  Blynk.run();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  Serial.print("Soil Temperature: ");
  Serial.println(temp);
  Serial.print("Temperature: ");
  Serial.println(t);
  Serial.print("Humidity: ");
  Serial.println(h);

  Blynk.virtualWrite(V3, t);  //V3 is for Temperature
  Blynk.virtualWrite(V4, h);  //V4 is for Humidity
  Blynk.virtualWrite(V2, temp); //Dallas Temperature

  soilMoistureValue = analogRead(SensorPin);  //put Sensor insert into soil
  Serial.println(soilMoistureValue);

  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);

  Blynk.virtualWrite(V1, soilmoisturepercent); //Soil Moisture sensor

  if (soilmoisturepercent > 100)
  {
    Serial.println("100 %");
    delay(1500);
    display.clearDisplay();

    // display Soil temperature
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 5);
    display.print("RH of Soil: ");
    display.print("100");
    display.print(" %");

    // display Air temperature
    display.setCursor(0, 20);
    display.print("Soil Temp: ");
    display.print(temp);
    display.print(" ");
    display.cp437(true);
    display.write(167);
    display.print("C");

    // display relative humidity of Soil
    display.setCursor(0, 35);
    display.print("Air Temp: ");
    display.print(t);
    display.print(" ");
    display.cp437(true);
    display.write(167);
    display.print("C");

    // display relative humidity of Air
    display.setCursor(0, 50);
    display.print("RH of Air: ");
    display.print(h);
    display.print(" %");

    display.display();
    delay(1500);
  }
  else if (soilmoisturepercent < 0)
  {
    Serial.println("0 %");
    delay(1500);
    display.clearDisplay();

    // display Soil temperature
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 5);
    display.print("RH of Soil: ");
    display.print("0");
    display.print(" %");

    // display Air temperature
    display.setCursor(0, 20);
    display.print("Soil Temp: ");
    display.print(temp);
    display.print(" ");
    display.cp437(true);
    display.write(167);
    display.print("C");

    // display relative humidity of Soil
    display.setCursor(0, 35);
    display.print("Air Temp: ");
    display.print(t);
    display.print(" ");
    display.cp437(true);
    display.write(167);
    display.print("C");

    // display relative humidity of Air
    display.setCursor(0, 50);
    display.print("RH of Air: ");
    display.print(h);
    display.print(" %");

    display.display();
    delay(1500);
  }
  else if (soilmoisturepercent >= 0 && soilmoisturepercent <= 100)
  {
    Serial.print(soilmoisturepercent);
    Serial.println("%");
    delay(1500);
    display.clearDisplay();

    // display Soil temperature
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 5);
    display.print("RH of Soil: ");
    display.print(soilmoisturepercent);
    display.print(" %");

    // display Air temperature
    display.setCursor(0, 20);
    display.print("Soil Temp: ");
    display.print(temp);
    display.print(" ");
    display.cp437(true);
    display.write(167);
    display.print("C");

    // display relative humidity of Soil
    display.setCursor(0, 35);
    display.print("Air Temp: ");
    display.print(t);
    display.print(" ");
    display.cp437(true);
    display.write(167);
    display.print("C");

    // display relative humidity of Air
    display.setCursor(0, 50);
    display.print("RH of Air: ");
    display.print(h);
    display.print(" %");

    display.display();
    delay(1500);
  }
  if (soilmoisturepercent >= 0 && soilmoisturepercent <= 30)
  {
    Serial.println("needs water, send notification");
    //send notification
    Blynk.logEvent("Plants need water... Pump is activated") ;
    digitalWrite(relay, LOW);
    digitalWrite(buzzer, HIGH);
    Serial.println("Motor is ON");
    WidgetLED PumpLed(V5);
    PumpLed.on();
    delay(1000);
  }
  else if (soilmoisturepercent > 30 && soilmoisturepercent <= 100)
  {
    Serial.println("Soil Moisture level looks good...");
    digitalWrite(relay, HIGH);
    digitalWrite(buzzer, LOW);
    Serial.println("Motor is OFF");
    WidgetLED PumpLed(V5);
    PumpLed.off();
    delay(1000);
  }

  rainState = digitalRead(rainPin);
  Serial.println(rainState);

  if (rainState == 0 && lastRainState == 0) {
    Serial.println("It's Raining outside!");
    Blynk.logEvent("It's Raining outside!") ;
    lastRainState = 1;
    delay(1000);
    //send notification

  }
  else if (rainState == 0 && lastRainState == 1) {
    delay(1000);
  }
  else {
    Serial.println("No Rains...");
    lastRainState = 0;
    delay(1000);
  }

  if (pinValue == HIGH)
  {
    getPirValue();
  }
  delay(100);
}
