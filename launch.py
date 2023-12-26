





 
from gpiozero.pins.pigpio import PiGPIOFactory
from gpiozero import Servo

import RPi.GPIO as gpio
import constants
import gpiozero
import pigpio
import time 
import func


# Modify these values based on your servo's specifications
min_pulse_width = 0.8 / 1000  # 0.5ms
max_pulse_width = 2.5 / 1000  # 2.5ms

pigpio_factory = PiGPIOFactory()
servo = Servo(constants.servoPin, min_pulse_width=min_pulse_width, max_pulse_width=max_pulse_width, pin_factory=pigpio_factory)

gpio.setmode(gpio.BCM)
gpio.setup(constants.rocketMosfetPin, gpio.OUT)


#try:
#    while True:
#        angle = float(input("Enter angle (0 to 180): "))
#        if 0 <= angle <= 180:
#            # Convert angle to servo value (-1 to 1)
#            servo_value = (angle - 90) / 90
#            servo.value = servo_value
#        else:
#            print("Angle must be between 0 and 180")
#        time.sleep(1)
#except KeyboardInterrupt:
#    pass




def launchLogic(dataDict):
	abort = False
	while abort == False:
		time.sleep(1)
		gpio.output(constants.rocketMosfetPin, gpio.LOW)
		status = func.readJson(constants.statusJsonPath, constants.lock)
		if status != None:
			start = status["launch"]
			if start == True:



				func.print_("Starting countdown!", dataDict)
				countDownTime = status["countDownTime"]
				for i in range(countDownTime, 0, -1): # LOOPS IN REVERSE (counts down)
					if i <= 10: 
						func.print_(f"Launch countdown: {i}", dataDict)
					
					time.sleep(1)
		
					status = func.readJson(constants.statusJsonPath, constants.lock)
					if status != None:
						start = status["launch"]
						if start == False:
							func.print_("Stopping coundown!", dataDict)
							abort = True
							break


					




				if abort == False: # IF THE FLIGHT WAS ABORTED
					if not None in dataDict.values() and "altitude" in dataDict and "time" in dataDict: # CHECKS IF WE ARE GETTING VALUES TO DETERMEN WHEN THE PARACHUTE SHULD LAUNCH
						maxHeight = 0
						gpio.output(constants.rocketMosfetPin, gpio.HIGH) # OPENS THE MOSFET THAT SHOOTS THE ROCKET
									
						while True:
					
						
							alt = dataDict["altitude"]
							launchTime = dataDict["time"] - countDownTime
						
							if alt > maxHeight:
								maxHeight = alt

							if maxHeight >= constants.openShuteHeight[0] and alt <= constants.openShuteHeight[1]:
								servoNum = func.convertDeg(0) 
								servo.value = servoNum # OPENS THE SERVO
								func.print_(f"Opened the Shute based on altitude: {alt}", dataDict)							

							elif launchTime >= constants.openShuteTime:
								servoNum = func.convertDeg(0)
								servo.value = servoNum # OPENS THE SERVO
								func.print_(f"\n Opened the Shute based on Time: {launchTime}", dataDict)
								print(alt, launchTime)				 
							
								time.sleep(10)
					else:
						func.print_(" \n There was an error with the dataDict!", dataDict)
						func.print_("Retrying Launch!", dataDict)
						func.print_(f"{dataDict} \n", dataDict)
				else:
					func.print("Launch aborted!", dataDict)
						

