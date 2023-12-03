from opensky_api import OpenSkyApi
from collections import defaultdict
from Sender import Sender
from DataModels import Record, Rectangle, Location, Region, Jsonifier, LocationArea, MovingObjectRecord
from Dto import Record2MovingObjectDto
import json
import time
from dataclasses import dataclass
import sys

api = OpenSkyApi(username='onrdmr2', password='425262026Asd')


global unitArea # now it is global



lastKnownState = defaultdict(str) # RecordStateDto
samplingCoefficient = 3 # this is value initiated the best 


# pipe configuration are made
sender = Sender("mypipe")

# You can set your own pipe place
# sender._setPipeDirectory("./pipe")

sender.writePipeFilePath2AppSetting()
sender.openFifo()



while True:

    unit = 0.3 # maximum length a plane can take in that moment
    s = api.get_states() 
    if(s == None) :
        time.sleep(10)
    
    else:
        for state in s.states:
            if(state.longitude == None or state.latitude == None or state.icao24 == None ):
                continue
            longitude = state.longitude
            latitute = state.latitude

            location = Location(longitude=longitude, latitude=latitute)

            oid = state.icao24
            region = Region(location=location, area=Rectangle(unit=unit))
            lastKnownRegion = None

            if(lastKnownState.get(oid) != None):
                lastKnownRegion = lastKnownState[oid].record.baseLocation


            if(lastKnownRegion != None):
                ## region value will be created
                height = abs(lastKnownRegion.latitude-region.center.latitude)
                width = abs(lastKnownRegion.longitude-region.center.longitude)
                region.area.width = width * samplingCoefficient ## samplingCoeffient is 
                region.area.height = height * samplingCoefficient

            record = Record(oid, location, region)

            lastKnownState[oid] = Record2MovingObjectDto(record=record)
            recordJsonData = lastKnownState[oid].toJsonRecord()
            sender.sendDataWithFlushBuffer(recordJsonData)
        time.sleep(5)
            

    



