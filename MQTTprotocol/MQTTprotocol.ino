#include "arduino_secrets.h"
#include <ArduinoBearSSL.h>
#include <Arduino_JSON.h>
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>              // change to #include <WiFi101.h> for MKR1000
#include <Arduino_MKRIoTCarrier.h> // provides Oplà carrier functions to the program

MKRIoTCarrier carrier; 

/////// Enter your sensitive data in arduino_secrets.h
const char ssid[]        = SECRET_SSID;
const char pass[]        = SECRET_PASS;
const char broker[]      = SECRET_BROKER;
const char* certificate  = SECRET_CERTIFICATE;
const char* key          = SECRET_KEY;


WiFiClient    wifiClient;            // Used for the TCP socket connection
BearSSLClient sslClient(wifiClient); // Used for SSL/TLS connection, integrates with ECC508
MqttClient    mqttClient(sslClient);

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial monitor to open

  // Set a callback to get the current time
  // used to validate the servers certificate
  ArduinoBearSSL.onGetTime(getTime);

  // Set the private key and the accompanying public 
  // certificate to use for client authencation 
  sslClient.setKey(key, certificate);
  
  // Optional, set the client id used for MQTT,
  // each device that is connected to the broker
  // must have a unique client id. The MQTTClient will generate
  // a client id for you based on the millis() value if not set
  mqttClient.setId("team06-device");

    
  // Set the message callback, this function is
  // called when the MQTTClient receives a message
  mqttClient.onMessage(onMessageReceived);


  // Initialize the carrier board of the Oplà kit
  CARRIER_CASE = false;
  carrier.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqttClient.connected()) {
    // MQTT client is disconnected, connect

    connectMQTT();
  }
}


unsigned long getTime() {
  // get the current time from the WiFi module
  return WiFi.getTime();
}

void connectWiFi() {
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(ssid);
  Serial.print(" ");

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the network");
  Serial.println();
}

void connectMQTT() {
  Serial.print("Attempting to MQTT broker: ");
  Serial.print(broker);
  Serial.println(" ");

  while (!mqttClient.connect(broker, 8883)) {
    // failed, retry
    //Serial.print("Secret key: ");
    //Serial.print(key);
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the MQTT broker");
  Serial.println();

  // subscribe to a topic
  // Pass Alternate ID
  mqttClient.subscribe("ack/team06-device");
  mqttClient.subscribe("commands/team06-device");
}

void onMessageReceived(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());     
  }
  Serial.println();
  Serial.println();
}
