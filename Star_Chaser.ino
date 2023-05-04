



#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include "SPI.h"
#include "FS.h"
#include "SD.h"

Adafruit_MPU6050 mpu;
Adafruit_BMP280 bmp; 

#define SEALEVELPRESSURE_HPA (1013.25) // SETS THE GROUND LEVEL PRESSURE, TO CALCULATE HEIGHT

const int buzzerPin = 13;
const int pwmChannel = 0;
const int freq = 200;

const int switchPin = 36;
const int sucsessBeppAmount = 3;
const int beepDelay = 400;


void buzzerNoise(int beepAmount, int beepDelay_){
  for (int i=0; i<beepAmount; i++){
    ledcWrite(pwmChannel, 255);
    delay(beepDelay_);
    ledcWrite(pwmChannel, 0);
    delay(beepDelay_);
  }
}

void init(Adafruit_MPU6050& mpuRef, Adafruit_BMP280& bmpRef){
  if (!mpuRef.begin()) { // CHECKS IF THE SENSOR IS NOT READY
    while (1) {  // GETS STUCK IN A WHILE LOOP
      Serial.println("Failed to find MPU6050 chip"); // PRINTS OUT THE ERROR  
      buzzerNoise(5, beepDelay); // CONSTANTLY BEEPS, TO LET THE USER KNOW SOMETHING IS WRONG
    }
  }

  if (!bmpRef.begin(0x77)) {
    while (1) {  // GETS STUCK IN A WHILE LOOP
      Serial.println("Could not find a valid BMP280 sensor, check wiring, address, sensor ID!"); // PRINTS OUT THE ERROR 
      buzzerNoise(5, beepDelay); // CONSTANTLY BEEPS, TO LET THE USER KNOW SOMETHING IS WRONG
    }
  }

  //if (!SDRef.begin()) {
  //  while (1) {  // GETS STUCK IN A WHILE LOOP
  //    Serial.println("SD Card Mount Failed!"); // PRINTS OUT THE ERROR  
  //    delay(1000);
  //  }
  //}
  //
  //uint8_t cardType = SDRef.cardType();
  //while (cardType == CARD_NONE) {  // GETS STUCK IN A WHILE LOOP IF THE SD CARD IS MISSING
  //  Serial.println("SD Card Mount Failed!"); // PRINTS OUT THE ERROR  
  //  delay(1000);
  //}

  buzzerNoise(sucsessBeppAmount, beepDelay); // BEEPS THREE TIMES, TO LET THE USER KNOW THE INITIALIZATION WAS SUCSESSFUL

}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

String createDataString(sensors_event_t &a, sensors_event_t &g, sensors_event_t &temp, Adafruit_BMP280 &bmp){
  String dataString = "";
  dataString+= String(a.acceleration.x) + ",";
  dataString+= String(a.acceleration.y) + ",";
  dataString+= String(a.acceleration.z) + ",";

  dataString+= String(g.gyro.x) + ",";
  dataString+= String(g.gyro.y) + ",";
  dataString+= String(g.gyro.z) + ",";

  dataString+= String(temp.temperature) + ",";
  dataString+= String(bmp.readTemperature()) + ",";
  dataString+= String(bmp.readPressure() / 100.0F) + ",";
  dataString+= String(bmp.readAltitude(SEALEVELPRESSURE_HPA)) + "\n";

  return dataString;
}



void setup(void) {
  Serial.begin(9600);

  while (!Serial) delay(10); // will pause Zero, Leonardo, etc until serial console opens

  if(!SD.begin()){
      Serial.println("Card Mount Failed");
      return;
  }

  pinMode(buzzerPin, OUTPUT);
  pinMode(switchPin, INPUT);
  ledcSetup(pwmChannel, freq, 16);
  ledcAttachPin(buzzerPin, pwmChannel);



  init(mpu, bmp); // CHECKS IF THERE IS ANY ERRORS

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); // SETS THE ACCELEROMETER RANGE TO +-8G
  mpu.setGyroRange(MPU6050_RANGE_500_DEG); // SETS THE GYROSCORE RANGE TO +-500 DEGREES PER SECOND
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ); // SETS THE LINE BANDWITDH TO 5HZ
  delay(100);
}


void loop() {


if (digitalRead(switchPin) == HIGH){ // IF THE USER ARMS THE ROCKET
  buzzerNoise(sucsessBeppAmount, beepDelay); //MAKES THE ARM NOISE

  while (digitalRead(switchPin) == HIGH){ // LOOPS WHILE THE ROCKET IS ARMED
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    String data = createDataString(a, g, temp, bmp); // RETURNS A STRING, CONTAINING THE SENSOR DATA IN CSV FORM
    appendFile(SD, "/data.txt", data.c_str()); // APPENDS THE DATA TO THE SD CARD

    delay(50); // A BIT OF DELAY

  }

}
  

}