from picamera import PiCamera

import constants
import time 
import func
import cv2

def startRecording(dataDict):
	try:
		camera = PiCamera()
		camera.rotation = 180
		camera.framerate = constants.videoFps
		camera.resolution = constants.videoRes	

		camera.start_recording(constants.videoPath + constants.videoName)	
		func.updateJsonValue(constants.statusJsonPath, ["status", "camera"], True, constants.lock)
		func.print_("Recording!", dataDict)
		while True: 
			time.sleep(3)
			status = func.readJson(constants.statusJsonPath, constants.lock)
			if status != None:
				if status["launch"] == False:
					break
					func.print_("Stopped recording!", dataDict)

		func.updateJsonValue(constants.statusJsonPath, ["status", "camera"], False, constants.lock)
		camera.stop_recording()
		camera.close()
	except:
		func.updateJsonValue(constants.statusJsonPath, ["status", "camera"], False, constants.lock)
		func.print_("\n CRITICAL ERROR IN CAMERA RECORING! ", dataDict)

