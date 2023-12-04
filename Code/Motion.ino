#include <Arduino.h>

#define PIR_PIN 4  // PIR sensor pin
#define SIM800_RX_PIN 16  // Use the RX pin of the HardwareSerial you want to use (e.g., 16)
#define SIM800_TX_PIN 17  // Use the TX pin of the HardwareSerial you want to use (e.g., 17)
#define BAUD_RATE 9600

HardwareSerial sim800Serial(1);  // Serial1 on ESP32, RX, TX

bool motionDetected = false;

void setup() {
  Serial.begin(115200);
  sim800Serial.begin(BAUD_RATE, SERIAL_8N1, SIM800_RX_PIN, SIM800_TX_PIN);
  
  pinMode(PIR_PIN, INPUT);
}

void loop() {
  checkPIR();

  if (motionDetected) {
    sendSMS("There is someone in your field!!");
    makeCall();
    motionDetected = false;  // Reset the flag
    delay(500);  // Wait for 1 seconds to avoid sending multiple messages in a short time
  }

  // Your other code here...

  delay(1000);  // Delay to avoid excessive checking
}


void checkPIR() {
  static int previousState = LOW;
  int currentState = digitalRead(PIR_PIN);
  
  if (currentState == HIGH && previousState == LOW) {
    Serial.println("Motion detected!");
    motionDetected = true;
  }
  
  previousState = currentState;
}


void sendSMS(String message) {
  sim800Serial.println("AT+CMGF=1");  // Set the SMS mode to text
  delay(1000);
  sim800Serial.print("AT+CMGS=\"+918928707201\"");  // Replace with the recipient's phone number
  sim800Serial.write('\r');
  delay(1000);
  sim800Serial.println(message);
  delay(1000);
  sim800Serial.write(26);  // CTRL+Z to send message
  delay(1000);
}

void makeCall() {
  sim800Serial.println("ATD+918928707201;");  // Replace with the phone number to call
  delay(5000);  // Wait for 5 seconds (adjust as needed)
  sim800Serial.println("ATH");  // Hang up the call
}
