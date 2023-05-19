from opensky_api import OpenSkyApi
import json
import time
import sys

api = OpenSkyApi(username='onrdmr2', password='425262026Asd')


def logMessage(message):
    sys.stderr.write("python: ")
    sys.stderr.write(message)
    sys.stderr.write("\n")
    sys.stderr.flush()


try:
    while True:
        s = api.get_states()

        if s == None:
            logMessage("empty")
            continue

        for state in s.states:
            if (state.longitude == None or state.latitude == None or state.icao24 == None):
                continue

            data = {
                "id": state.icao24,
                "baseLocation": {
                    "longitude": state.longitude,
                    "latitude": state.latitude,
                },
            }

            sys.stdout.write(json.dumps(data))
            sys.stdout.write('\n')
            sys.stdout.flush()
            time.sleep(0.1)
        time.sleep(1)
except Exception as e:
    logMessage(str(e))
except KeyboardInterrupt:
    logMessage("exiting")


# {"pointLow": {"longitude": 50.00,"latitude": 50.00}, "pointHigh": {"longitude": 60.00,"latitude": 60.00}}
