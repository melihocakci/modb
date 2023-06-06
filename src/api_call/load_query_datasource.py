from opensky_api import OpenSkyApi
import time
import sys
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


api = OpenSkyApi(username='onrdmr2', password='425262026Asd')

try:
    counter = 0
    while True:
        s = api.get_states()

        if s == None:
            logMessage("empty")
            continue

        for state in s.states:
            counter += 1
            if (counter % 20 == 0):
                printQuery()

            if (state.longitude == None or state.latitude == None or state.icao24 == None):
                continue

            print(1, state.icao24, state.longitude, state.latitude)

        time.sleep(10)
except Exception as e:
    logMessage(str(e))
except KeyboardInterrupt:
    logMessage("exiting")
