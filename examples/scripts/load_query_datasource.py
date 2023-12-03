import time
import sys
import json
import random


def logMessage(message):
    sys.stderr.write("python: ")
    sys.stderr.write(message)
    sys.stderr.write("\n")
    sys.stderr.flush()


def printQuery():
    x = random.uniform(-120, -80)
    y = random.uniform(30, 50)
    dx = random.uniform(0.2, 1.0)
    dy = random.uniform(0.2, 1.0)

    print(2, x, y, x + dx, y + dy)


try:
    with open("dataset_json_long.txt") as file:
        counter = 0
        for line in file:
            counter += 1
            if (counter % 20 == 0):
                printQuery()

            obj = json.loads(line)

            print(1, obj['id'], obj['baseLocation']
                  ['longitude'], obj['baseLocation']['latitude'])

except Exception as e:
    logMessage(str(e))
except KeyboardInterrupt:
    logMessage("exiting")
