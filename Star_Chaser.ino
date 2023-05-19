
#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include "SPI.h"
#include "FS.h"
#include <SD.h>



Adafruit_MPU6050 mpu;
Adafruit_BMP280 bmp; 

#define SEALEVELPRESSURE_HPA (1026.50) // SETS THE GROUND LEVEL PRESSURE, TO CALCULATE HEIGHT

const int buzzerPin = 13;
const int pwmChannel = 0;
const int freq = 200;


const int switchPin = 36;
const int sucsessBeppAmount = 3;
const int beepDelay = 400;

const int relayPin = 12; 
const int countdownTime = 7*60;

const int servoPin = 32;  
const int minPulseWidth = 2700; // Minimum pulse width for most servos (in microseconds)
const int maxPulseWidth = 8000; // Maximum pulse width for most servos (in microseconds)
const int servoFrequency = 50; // Servo control frequency (50 Hz)
const int servoChannel = 1; // PWM channel for the servo (can be any channel from 0 to 15)

//const int minPul  seWidth = 1900; // Minimum pulse width for most servos (in microseconds)
//const int maxPulseWidth = 5150; // Maximum pulse width for most servos (in microseconds)

const int heightArrLen = 5; // SETS THE LENGTH OF THE HEIGHT ARRAY, IF ALL OF THE VALUES IN THE HEIGHT ARRAY IS DECREASING, SHULD THE PARACHUTE RELEASE
int latestHeight[heightArrLen] = {0, 0, 0, 0, 0}; // INITIALIZES THE LATEST HEIGHT ARRAY, FOR RELEASING THE PARACHUTE
int maxAltitude; // KEEPS TRACK OF THE MAX ALTITUDE





void buzzerNoise(int beepAmount, int beepDelay_){
  for (int i=0; i<beepAmount; i++){
    ledcWrite(pwmChannel, 255);
    delay(beepDelay_);
    ledcWrite(pwmChannel, 0);
    delay(beepDelay_);
  }
}

void countdown(int contdownTime, int beepDelay, bool print_){
  int time = contdownTime; // MAKES A CLONE OF THE INPUT VAIRBALE, SO I CAN SUBTRACT FORM THE VARIABLE

  while (time >= 0 and digitalRead(switchPin) == HIGH){ // WHILE TIME IS NOT ZERO AND THE ARM SWITCH IS STILL SWITCHED
    if (print_ == true){
      Serial.printf("Countdown: %d \n", time); // PRINTS OUT THE COUNTDOWN TIME
    }

    if (time <= 9 or time % 10 == 0){ // IF THE NUMBER IS LESS THAN 10 OR 10,20,30,40 OSV
      buzzerNoise(1, beepDelay ); // BEEPS ONCE
      delay(1000 - beepDelay*2); // SLEEPS FOR ONE SECOND, MINUS THE TIME IT TOOK FOR THE BEEPER TO BEEP

    }else{
      delay(1000); // SLEEPS FOR A SECOND
    }

    time -=1; // SUBTRACTS ONE FROM THE TIME VARIBLE, TO SIGNAL THAT A SECOND HAS GONE BY

  }
}

void init(Adafruit_MPU6050& mpuRef, Adafruit_BMP280& bmpRef){

  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(switchPin, INPUT);
  ledcSetup(pwmChannel, freq, 16);
  ledcAttachPin(buzzerPin, pwmChannel);

  ledcSetup(servoChannel, servoFrequency, 16);
  ledcAttachPin(servoPin, servoChannel);

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

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        1+1;
    } else {
        Serial.printf("Failed to append message: %d \n", message);
    }
    file.close();
}

String createDataString(sensors_event_t &a, sensors_event_t &g, sensors_event_t &temp, Adafruit_BMP280 &bmp, float elapsedTime){
  String dataString = "";
  dataString+= String(elapsedTime) + ",";
  dataString+= String(a.acceleration.x) + ",";
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

String createVariableString(int maxAltitude, int altitude, int latestHeight0, int latestHeight1, int latestHeight2, int latestHeight3, int latestHeight4){
  String varString = "";

  varString += String(maxAltitude) + ",";
  varString += String(altitude) + ",";
  varString += String(latestHeight0) + ",";
  varString += String(latestHeight1) + ",";
  varString += String(latestHeight2) + ",";
  varString += String(latestHeight3) + ",";
  varString += String(latestHeight4) + ",";

  return varString; 
}

void servoPos(int pos){ 
    int pulseWidth = map(pos, 0, 180, minPulseWidth, maxPulseWidth);
    ledcWrite(servoChannel, pulseWidth);
}


void pushFirstIndexArr(int* array, int newValue, int arrLen){
  for (int i=0; i<arrLen-1; i++){ // LOOPS THE SAME AMOUNT OF TIMES AS THE LENGTH OF THE ARRAY -1
    array[i+1] = array[i]; // SETS THE FIRST ARRAY POSITION TO BE THE SECOND, AND THE SECOND TO THIRD AND SO ON
  } 
  array[0] = newValue; // SETS THE FIRST INDEX TO BE THE NEW VALUE
}

bool checkReleaseParachute(int* latestHeightArr, int arrLen, int maxAltitude){

  if (maxAltitude > 36 and latestHeightArr[0] < 35 ){ // IF THE ROCKET HAS FLOWN HEIGHER THAN 20 METERS AND IS LESS THAN 15 METERS
    appendFile(SD, "/data.txt", "$$ Fired height checking control! \n");
    return true; // RELEASE THE PARACHUTE
  }

  for (int i=0; i<arrLen-1; i++){ // LOOPS OVER ALL OF THE ITEMS IN latestHeightArr
    if (latestHeightArr[i] <= latestHeightArr[i+1]) { // IF TWO NEIGBORING latestHeightArr IS IS NOT DECREASING ALTITUDE
      return false; // DONT RELEASE THE PARACHUTE
      }
    }
  appendFile(SD, "/data.txt", "$$ Fired List atatude control! \n");
  return true; // IF THE ROCKET IS DECREASING ALTITUDE, THEN RELEASE THE PARACHTUE
}


void setup(void) {
  Serial.begin(9600);

  while (!Serial) delay(10); // will pause Zero, Leonardo, etc until serial console opens

  if(!SD.begin()){
      Serial.println("Card Mount Failed");
      return;
  }

  init(mpu, bmp); // CHECKS IF THERE IS ANY ERRORS, AND SETS UP ALL OF THE PINS
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); // SETS THE ACCELEROMETER RANGE TO +-8G
  mpu.setGyroRange(MPU6050_RANGE_500_DEG); // SETS THE GYROSCORE RANGE TO +-500 DEGREES PER SECOND
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ); // SETS THE LINE BANDWITDH TO 5HZ

  Serial.printf("\nGround Level Pressure: %f \n", bmp.readPressure() / 100.0F);
  servoPos(100); // LOCKS THE SERVO, TO MAKE SURE THE PARACHUE HATCH IS CLOSED
  delay(100);
}
void loop() {

 
  if (digitalRead(switchPin) == HIGH){ // IF THE USER ARMS THE ROCKET
    buzzerNoise(sucsessBeppAmount, beepDelay); //MAKES THE ARM NOISE
    delay(500); 

    countdown(countdownTime, beepDelay, true); // MAKES THE COUNTDOWN TIMER
    servoPos(0); // OPENS THE SERVO, TO RELEASE THE PARACHUTE

    appendFile(SD, "/data.txt", "time,a.acceleration.x,a.acceleration.y,a.acceleration.z,g.gyro.x,g.gyro.y,g.gyro.z,temp1,temp2,pressure,altitude \n"); // APPENDS THE SEPERATOR TO THE CSV FILE
    unsigned long startTime = millis(); 

    digitalWrite(relayPin, HIGH); // SWITCHES ON THE RELAY, TO START THE ROCKET MOTOR
    while (digitalRead(switchPin) == HIGH){ // LOOPS WHILE THE ROCKET IS ARMED
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      float elapsedTime = (float)(millis() - startTime) / 1000.0; // CALCULATES THE ELAPSED TIME SINCE THE ROCKET STARTED IN SECONDS
      String data = createDataString(a, g, temp, bmp, elapsedTime); // RETURNS A STRING, CONTAINING THE SENSOR DATA IN CSV FORM
      appendFile(SD, "/data.txt", data.c_str()); // APPENDS THE DATA TO THE SD CARD

      Serial.println(data);

      int altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA); // GETS THE CURRENT ALTITUDE
      pushFirstIndexArr(latestHeight, altitude, heightArrLen); // ADDS THE CURRENT HEIGHT TO THE FIRST INDEX OF THE latestHeight ARRAY
      bool releasePar = checkReleaseParachute(latestHeight, heightArrLen, maxAltitude); // FIGURES OUT IF THE PARACHUTE SHULD BE RELEASED

      if (releasePar == true){ // IF THE PARACHUTE SHULD BE RELEASED
        servoPos(0); // OPENS THE SERVO, TO RELEASE THE PARACHUTE
        appendFile(SD, "/data.txt", " $$ Release Parachute \n"); // APPENDS THE DATA TO THE SD CARD
        String varValues = createVariableString(maxAltitude, altitude, latestHeight[0], latestHeight[1], latestHeight[2], latestHeight[3], latestHeight[4]);
        appendFile(SD, "/data.txt", varValues.c_str()); // APPENDS THE DATA TO THE SD CARD
      }
  
      if (altitude > maxAltitude){ // IF THE CURRENT ALTITUDE IS GREATER THAN THE MAX ALTITUDE
        maxAltitude = altitude; // SETS THE NEW MAX ALITTUDE
      }



    }
    digitalWrite(relayPin, LOW); // OPENS THE RELAY
  }

  delay(500);
}