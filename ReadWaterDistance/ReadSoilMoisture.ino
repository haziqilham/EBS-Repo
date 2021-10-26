#include <Arduino_MKRIoTCarrier.h> // provides Oplà carrier functions to the program


void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial monitor to open
}

void loop() {
  // read raw moisture value
  int raw_moisture = analogRead(A5);
 
  // map raw moisture to a scale of 0 - 100
  int soilMoisture = map(raw_moisture, 0, 1023, 100, 0);
  Serial.print("Analog value: ");
  Serial.print(raw_moisture);
  Serial.print(" Soil moisture: ");
  Serial.println(soilMoisture);
  delay(1000);
}
