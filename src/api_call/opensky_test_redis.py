from opensky_api import OpenSkyApi
from collections import defaultdict
from Sender import Sender
from DataModels import Record, Rectangle, Location, Region
import json
import time
from dataclasses import dataclass

from redis import StrictRedis

api = OpenSkyApi(username='onrdmr2', password='425262026Asd')


global unitArea # now it is global

@dataclass
class RecordStateDto:

    def __init__(self, record: Record, velocity: float) -> None:
        self.record = record
        self.velocity = velocity

    def toJsonRecord(self):
        return json.dumps(record.dict())

    record : Record
    velocity : float


# extension method
class StrictRedis(StrictRedis):
    def getDataJson(self, oid : str):
        if(self.exists(oid)):
            data = self.get(oid)
            return data
        return None
        return json.load(data)
    def writeDataJson(self,oid : str, data: json ):
        self.set(oid, data)



# this will exceed in the memory solution for this is redis cache
# redis cache suits for live api temporary storage
# lastKnownState = defaultdict(str) # RecordStateDto
lastKnownState = StrictRedis(host='localhost', port=6379, db=0)

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


    for state in s.states:
        if(state.longitude == None or state.latitude == None or state.icao24 == None ):
            continue
        longitude = state.longitude
        latitute = state.latitude

        location = Location(longitude=longitude, latitude=latitute)

        oid = state.icao24
        region = Region(location=location, area=Rectangle(unit=unit))
        lastKnownRegion = None

        if(lastKnownState.getDataJson(oid) != None):
            lastKnownRegion = lastKnownState[oid].record.baseLocation


        if(lastKnownRegion != None):
            ## region value will be created
            height = abs(lastKnownRegion.latitude-region.center.latitude)
            width = abs(lastKnownRegion.longitude-region.center.longitude)
            region.area.width = width * samplingCoefficient ## samplingCoeffient is 
            region.area.height = height * samplingCoefficient

        record = Record(oid, location, region)

        recordDto = RecordStateDto(record=record, velocity=state.velocity)
        
        recordJsonData = recordDto.toJsonRecord()
        
        lastKnownState.writeDataJson(recordJsonData)

        sender.sendDataWithFlushBuffer(recordJsonData)
        

    
    time.sleep(1)



