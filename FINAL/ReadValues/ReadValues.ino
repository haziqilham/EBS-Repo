#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>
#include <Wire.h>
#include <Arduino_MKRIoTCarrier.h> // provides Oplà carrier functions to the program
#include <Servo.h>

MKRIoTCarrier carrier;
SFEVL53L1X distanceSensor;
Servo Servo1;

float temperature;
float humidity;
int r, b, g, a;
int servoPin = A3;

void setup(void)
{
  Wire.begin();

  Serial.begin(115200);
  Serial.println("VL53L1X Qwiic Test");
  // Initialize the carrier board of the Oplà kit for temperture and humidity
  CARRIER_CASE = false;
  carrier.begin();

  if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("Sensor online!");

  Servo1.attach(servoPin);
}


void loop(void)
{
  // Water level reading
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }
  int distance = distanceSensor.getDistance();
  distanceSensor.clearInterrupt();
  distanceSensor.stopRanging();
  Serial.print("Distance(mm): ");
  Serial.println(distance);

  // Soil moisture reading
  int raw_moisture = analogRead(A5);
  int soilMoisture = map(raw_moisture, 0, 1023, 100, 0);
  Serial.print("Soil moisture: ");
  Serial.println(soilMoisture);

  // Environment: temperature and humidity reading
  temperature = carrier.Env.readTemperature();
  humidity    = carrier.Env.readHumidity();
  Serial.print("Temperature = ");
  Serial.println(temperature);
  Serial.print("Humidity = ");
  Serial.println(humidity);

  // Light intensity reading
  while (!carrier.Light.colorAvailable()) 
  {
    delay(5);
  }
  carrier.Light.readColor(r, g, b, a);
  Serial.print("Light intensity = ");
  Serial.println(a);
  delay(1000);

   // Make servo go to 0 degrees 
   Servo1.write(0); 
   delay(1000); 
   // Make servo go to 90 degrees 
   Servo1.write(90); 
   delay(1000); 
   // Make servo go to 180 degrees 
   Servo1.write(180); 
   delay(1000); 
   
  Serial.println();
}
