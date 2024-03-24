#include<Wire.h>
#include "FirebaseESP8266.h"
#include<ESP8266WiFi.h>


#include <Servo.h>
#define ANALOG_PIN A0

Servo myServo;  
#define TRIGGER 5
#define ECHO    4


const int ledPin = 13;
char FIREBASE_AUTH [] = "stMTHDFUc6ujIpcYo1QEclfdB2Bgv4blDWWgWhM5"; // Your Firebase Web API Key
char FIREBASE_HOST [] = "smartduskbin-default-rtdb.firebaseio.com"; // Your Firebase Host URL
char WIFI_SSID [] = "Dialog 4G 035";     // Your WIFI SSID
char WIFI_PASSWORD [] = "5E5f3Ae3"; // Your WIFI Password

FirebaseData firebaseData;


void setup() {
  Serial.begin(9600);
  myServo.attach(D3);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial1.begin(115200);
  pinMode(ledPin,OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
}

void loop() {
  Read();
  if (checkDist() < 50) {
    Serial.println("Object detected!");
    myServo.write(120);  // Move the servo to 90 degrees
    delay(1000);        // Allow time for the object to move away
    while(checkDist() < 50) {
      // Object still detected, wait longer or take appropriate action
      delay(1000);
    }
    myServo.write(0);   // Move the servo back to the start position (0 degrees)
    delay(1000);
  }
  delay(1000);
}

long checkDist() {
  long duration, distance;

  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2); 
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIGGER, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = (duration / 2) / 29.1;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}

void Read(){
  int sensorValue = analogRead(ANALOG_PIN);
  float voltage = sensorValue * (3.3 / 1023.0); // Convert ADC reading to voltage (assuming 3.3V VCC)

  Serial.print("Analog reading: ");
  Serial.print(sensorValue);
  Serial.print(", Voltage: ");
  Serial.println(voltage);

  if (voltage > 1) {
  Serial.println("start fb transmission");
  Firebase.setString(firebaseData, "/data", "Dust Bin B001 have filled"); //This will create a path as 'data' and save the value on Firebase
  delay(5000); 
  while(voltage<1){
    delay(1000);
    sensorValue = analogRead(ANALOG_PIN);
    voltage = sensorValue * (3.3 / 1023.0); // Convert ADC reading to voltage (assuming 3.3V VCC)
    Serial.print("Analog reading: ");
    Serial.print(sensorValue);
    Serial.print(", Voltage: ");
    Serial.println(voltage);
  }
  Firebase.setString(firebaseData, "/data", "Dust Bin B001 have not filled");
}

}
