
import constants
import json 
import time 
import cv2

def readJson(path, lock):
	for i in range(10):
		try:
			with lock:
        			with open(path) as f:
          				data = json.load(f)
			return data


		except:
			time.sleep(0.2)


def writeJson(path, data, lock):
	for i in range(10): 
		try:
			with lock:
				with open(path, 'w') as outfile:
					json.dump(data, outfile, indent=4, sort_keys=True)
		except:
			time.sleep(0.2)


def updateJsonValue(jsonPath, keyPath, newValue, lock, json_data=None):
    """
    Update a value in a nested JSON object.

    :param json_data: The JSON data as a dictionary.
    :param key_path: The path to the key as a list. Each element is a key in the nested structure.
    :param new_value: The new value to set for the specified key.
    :return: Updated JSON data.
    """


    if json_data == None: # GETS THE PREVIS JSON DATA 
        json_data = readJson(jsonPath, lock)


    temp = json_data
    for key in keyPath[:-1]:  # Go through the path to find the relevant dict
        temp = temp[key]
    temp[keyPath[-1]] = newValue  # Update the value

    writeJson(jsonPath, json_data, lock)


convertDeg = lambda deg: (deg - 90) / 90 # CONVERTS THE ANGLE TO SERVO VALUE (-1 TO 1)
