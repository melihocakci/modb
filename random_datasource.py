import json
import sys
import random

def logMessage(message):
    sys.stderr.write("python: ")
    sys.stderr.write(message)
    sys.stderr.write("\n")
    sys.stderr.flush()


try:
    for i in range(1000000):
        longitude = random.uniform(-150.0, 150.0)
        latitude = random.uniform(-75.0, 75.0)

        data = {
            "id": str(i),
            "baseLocation": {
                "longitude": longitude,
                "latitude": latitude,
            },
        }

        sys.stdout.write(json.dumps(data))
        sys.stdout.write('\n')
        sys.stdout.flush()

except Exception as e:
    logMessage(str(e))
except KeyboardInterrupt:
    logMessage("exiting")

