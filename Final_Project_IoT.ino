#define BLYNK_TEMPLATE_ID "TMPL6jQTiYsdd"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "-7uhmxO5GwfEY7yv9kSsWbRsOzqvcHDM"

#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <DHT.h>

char ssid[] = "yourwifi";
char pass[] = "yourpassword";

#define EspSerial Serial1

#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

#define DHT11_PIN 4      // Digital pin for DHT11
#define DHT22_PIN 5      // Digital pin for DHT22

DHT dht11(DHT11_PIN, DHT11);  // DHT11 sensor
DHT dht22(DHT22_PIN, DHT22);  // DHT22 sensor

BlynkTimer timer;

const int pirPin = 6;
const int trigPin = 9;
const int echoPin = 10;

float distance, duration;

void sendDHT11Sensor()
{
  float h11 = dht11.readHumidity();
  float t11 = dht11.readTemperature();

  if (isnan(h11) || isnan(t11)) {
    Serial.println("Failed to read from DHT11 sensor!");
    return;
  }

  // Sending data from DHT11 to V2 and V4
  Blynk.virtualWrite(V2, h11);
  //Serial.println(h11);
  Blynk.virtualWrite(V4, t11);
  //Serial.println(t11);
}

void sendDHT22Sensor()
{
  float h22 = dht22.readHumidity();
  float t22 = dht22.readTemperature();

  if (isnan(h22) || isnan(t22)) {
    Serial.println("Failed to read from DHT22 sensor!");
    return;
  }

  // Sending data from DHT22 to V6 and V5
  Blynk.virtualWrite(V6, h22);
  //Serial.println(h22);
  Blynk.virtualWrite(V5, t22);
  //Serial.println(t22);
}

void PIR() {
  int pirValue = digitalRead(pirPin);

  if (pirValue == HIGH) {
    Serial.println("Motion detection!");
    Blynk.virtualWrite(V7, "MOTION DETECTED!");
    activateUltrasonicSensor();
  }
  else {
    Blynk.virtualWrite(V7, "No motion Detected.");
  }
}

void activateUltrasonicSensor() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  float distance = (duration * 0.034) / 2;
  Serial.print("Distance: ");
  Serial.println(distance);

  Blynk.virtualWrite(V8, distance);
}

void setup()
{
  pinMode(pirPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);

  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass);

  dht11.begin();
  dht22.begin();

  timer.setInterval(4000L, sendDHT11Sensor);
  timer.setInterval(4000L, sendDHT22Sensor);
  timer.setInterval(6000L, PIR);

}

void loop()
{
  Blynk.run();
  timer.run();

}