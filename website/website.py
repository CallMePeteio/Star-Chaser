from flask import render_template
from flask import request
from flask import abort
from flask import Flask

import constants 
import logging
import json
import time
import os

app = Flask(__name__)

log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)


def readJson(path):
    for i in range(10):
        try:
            with open(path) as f:
                data = json.load(f)

                return data
        except:
            pass

def writeJson(path, data):
    for i in range(10):
        try:
            with open(path, 'w') as outfile:
                json.dump(data, outfile, indent=4, sort_keys=True)
        except:
            pass

def updateJsonValue(jsonPath, keyPath, newValue, json_data=None):
    """
    Update a value in a nested JSON object.

    :param json_data: The JSON data as a dictionary.
    :param key_path: The path to the key as a list. Each element is a key in the nested structure.
    :param new_value: The new value to set for the specified key.
    :return: Updated JSON data.
    """


    if json_data == None: # GETS THE PREVIS JSON DATA 
        json_data = readJson(jsonPath)


    temp = json_data
    for key in keyPath[:-1]:  # Go through the path to find the relevant dict
        temp = temp[key]
    temp[keyPath[-1]] = newValue  # Update the value

    writeJson(jsonPath, json_data)



@app.route('/', methods=["POST", "GET"])
def home():

    if request.method == "POST":

        try:
            if request.form.get('action') == 'launch':
                print("Launch botton was pressed")
                updateJsonValue(constants.statusJsonPath, ["launch"], True)
                updateJsonValue(constants.statusJsonPath, ["startTime"], time.time())

            elif request.form.get('action') == 'abort':
                print("Abort button was pressed")
                updateJsonValue(constants.statusJsonPath, ["launch"], False)
        except:
            abort(500)

    return render_template("index.html")



@app.route("/status", methods=["GET"])
def status():
    currentStatus = readJson(constants.statusJsonPath)
    if currentStatus != None:	
        if currentStatus["launch"] == True:
            elapsedTimeSinceBtn = (time.time() - currentStatus["startTime"]) / 60
            countDownTime = currentStatus["countDownTime"] / 60
            if elapsedTimeSinceBtn >= countDownTime:
                currentStatus["launchTime"] = round(elapsedTimeSinceBtn - countDownTime, 2) # CONVERTS TO MIN

    else:
        return abort(500)
        

    return currentStatus



app.run(debug=True, host="0.0.0.0")





"""
{
    "launch": false,
    "startTime": 1703252392.8258853,
    "status": {
        "camera": false,
        "datalogging": false
    }
}



"""
