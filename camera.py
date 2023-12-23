from picamera import PiCamera

import constants
import time 
import func
import cv2

def startRecording():
	camera = PiCamera()
	camera.rotation = 180
	camera.framerate = constants.videoFps
	camera.resolution = constants.videoRes	

	camera.start_recording(constants.videoPath + constants.videoName)	
	func.updateJsonValue(constants.statusJsonPath, ["status", "camera"], True, constants.lock)
	print("Recording!")
	while True: 
		time.sleep(3)
		status = func.readJson(constants.statusJsonPath, constants.lock)
		if status != None:
			if status["launch"] == False:
				break
				print("Stopped recording!")

	func.updateJsonValue(constants.statusJsonPath, ["status", "camera"], False, constants.lock)
	camera.stop_recording()
	camera.close()

#convert_h264_to_mp4(videoPath + "video.h264", videoPath + 'output_video.mp4')
