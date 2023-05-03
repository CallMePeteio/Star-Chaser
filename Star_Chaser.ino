// Basic demo for accelerometer readings from Adafruit MPU6050

// ESP32 Guide: https://RandomNerdTutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/
// ESP8266 Guide: https://RandomNerdTutorials.com/esp8266-nodemcu-mpu-6050-accelerometer-gyroscope-arduino/
// Arduino Guide: https://RandomNerdTutorials.com/arduino-mpu-6050-accelerometer-gyroscope/

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;



void init(Adafruit_MPU6050& mpuRef){
  if (!mpuRef.begin()) { // CHECKS IF THE SENSOR IS NOT READY
    while (1) {  // GETS STUCK IN A WHILE LOOP
      Serial.println("Failed to find MPU6050 chip"); // PRINTS OUT THE ERROR  
      delay(10);
    }
  }
}

void setup(void) {
  Serial.begin(9600);

  while (!Serial) delay(10); // will pause Zero, Leonardo, etc until serial console opens
  init(mpu);




  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); // SETS THE ACCELEROMETER RANGE TO +-8G
  mpu.setGyroRange(MPU6050_RANGE_500_DEG); // SETS THE GYROSCORE RANGE TO +-500 DEGREES PER SECOND
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ); // SETS THE LINE BANDWITDH TO 5HZ
  delay(100);
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
  delay(500);
}