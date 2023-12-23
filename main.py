



import threading
import constants
import launch
import func
import time 
import os


dataDict = {'time': None, 'altitude': None, 'atmoPressure': None, "temp": None, "accelerationX": None, "accelerationY": None, "accelerationZ": None, "gyroX": None, "gyroY": None, "gyroZ": None} 

def startModules():

	import datalogging
	import camera	

	print("Starting Threads!")
	threading.Thread(target=datalogging.startLogging, args=(dataDict,)).start()
	threading.Thread(target=camera.startRecording, args=()).start()
	threading.Thread(target=launch.launchLogic, args=(dataDict,)).start()
	
	

def checkStartModules():
	while True:
		status = func.readJson(constants.statusJsonPath, constants.lock)
		if status != None:
			start = status["launch"]
	
			if start == True: 
				startModules()
			  
			
				while func.readJson(constants.statusJsonPath, constants.lock)["launch"]: 
					time.sleep(1)
	

			print("checked")
			time.sleep(1)


func.writeJson(constants.statusJsonPath, constants.defaultStatusJson, constants.lock)

threading.Thread(target=checkStartModules, args=()).start()

os.system("sudo python " + constants.websiteFilePath)
