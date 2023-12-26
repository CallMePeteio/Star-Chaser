

import csv
import func
import time
import board
import constants
import adafruit_bmp280
import adafruit_mpu6050


i2c = board.I2C()
bmp280 = adafruit_bmp280.Adafruit_BMP280_I2C(i2c)
mpu = adafruit_mpu6050.MPU6050(i2c)

bmp280.sea_level_pressure = 1013.23 # SETS A DUMMY PRESSURE

startAlt = bmp280.altitude # GETS THE START ALT, USED FOR CALCULATING ALT FROM LAUNCH PAD

def startLogging(dataDict):
	totalLoggings, firstRun = 0, True
	startTime = time.time()
	while True:
		with open(constants.dataPath + constants.dataName, 'a') as csvfile:
			writer = csv.DictWriter(csvfile, fieldnames=constants.fieldNames)
			for _ in range(constants.writeRows):
				totalLoggings+=1 
				altFromLaunch = bmp280.altitude - startAlt # CALCULATES THE CURRENT ALTITUDE
				temp = (bmp280.temperature + mpu.temperature) / 2 # GETS THE AVRAGE TEMPERATRUE
				
				dataList = [time.time() - startTime, altFromLaunch, bmp280.pressure, temp, mpu.acceleration[0],  mpu.acceleration[1],  mpu.acceleration[2], mpu.gyro[0], mpu.gyro[1], mpu.gyro[2]]

				if len(dataDict) == len(dataList):
					for i, (key, _) in enumerate(dataDict.items()): # LOOPS OVER ALL OF THE DIFFRENT KEYS IN THE DICT
						dataDict[key] = dataList[i]
				else:
					func.print_("\n Datalist and dataDict does not have the equal amount of items", dataDict) 
				
				writer.writerow(dataDict)

		

		func.print_(f"Sucsessfully logged data: {totalLoggings}", dataDict)
		
		status = func.readJson(constants.statusJsonPath, constants.lock)
		if status["launch"] == False:
			break

		if firstRun == True:
			func.updateJsonValue(constants.statusJsonPath, ["status", "datalogging"], True, constants.lock, json_data=status)
			firstRun = False

	
	func.updateJsonValue(constants.statusJsonPath, ["status", "datalogging"], False, constants.lock)
	firstRun = False
	
		#print(altFromLaunch, pressure, temp, acceleration, gyro)

	
