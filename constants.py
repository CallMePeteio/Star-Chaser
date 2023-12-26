




import threading
import datetime
import logging

defaultStatusJson = {
    "countDownTime": 60, 
    "launch": False,
    "startTime": 1703252392.8258853,
    "status": {
        "camera": False,
        "datalogging": False
    }
}





fieldNames = ["time", "altitude", "atmoPressure", "temp", "accelerationX", "accelerationY", "accelerationZ", "gyroX", "gyroY", "gyroZ"]
writeRows = 30

videoName = str(datetime.datetime.now()).replace(" ", "") + ".h264"
videoPath = "/home/pi/code/videos/"

videoFps = 60
videoRes = (1280, 720)

dataName = str(datetime.datetime.now()).replace(" ", "") + ".csv"
dataPath = "/home/pi/code/videos/"

statusJsonPath = "/home/pi/code/status.json"
websiteFilePath = "/home/pi/code/website/website.py"

logFilePath = "/home/pi/code/videos/log.txt"

rocketMosfetPin = 22
servoPin = 6


openShuteHeight = (34, 31) # IF THE ROCKET HAS FLOWN OVER 34 METERS AND CURRENT HEIGHT IS UNDER 31 METERS
openShuteTime = 5 # IF THE ROCKET HAS FLOWN MORE THAN 5.5 SECONDS

timePrintPlacment = 40
lock = threading.Lock()




