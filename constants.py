




import threading
import datetime
import logging

defaultStatusJson = {
    "countDownTime": 30, 
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

videoFps = 90
videoRes = (640, 480)

dataName = str(datetime.datetime.now()).replace(" ", "") + ".csv"
dataPath = "/home/pi/code/data/"

statusJsonPath = "/home/pi/code/status.json"
websiteFilePath = "/home/pi/code/website/website.py"

rocketMosfetPin = 22
servoPin = 6


openShuteHeight = (34, 31) # IF THE ROCKET HAS FLOWN OVER 34 METERS AND CURRENT HEIGHT IS UNDER 31 METERS
openShuteTime = 5 # IF THE ROCKET HAS FLOWN MORE THAN 5.5 SECONDS


lock = threading.Lock()




