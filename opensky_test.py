from opensky_api import OpenSkyApi
from collections import defaultdict

import json
import time

api = OpenSkyApi(username='onrdmr2', password='425262026Asd')


global unitArea # now it is global



class Location:

    def __init__(self, longitude: float, latitude: float) -> None:
        self.longitude = longitude
        self.latitude = latitude

    longitude : float
    latitude : float

class Rectangle:
    
    def __init__(self, width=None, height=None, unit=None) -> None:
        if unit == None:
            self.width = width
            self.height = height
        else:
            self.width = unit
            self.height = unit


    width: float 
    height: float

class Region:
    def __init__(self, location: Location, area: Rectangle ) -> None:
        self.center = location
        self.area = area
 
    center : Location
    area : Rectangle

class Record:

    def __init__(self, oid: str,  baseLocation: Location, probabilityRegion: Region) -> None:
        self.oid = oid
        self.baseLocation = baseLocation
        self.probabilityRegion = probabilityRegion

    baseLocation : Location
    probabilityRegion : Rectangle
    oid : int

    def __iter__(self):
        yield from {
            "oid": self.oid,
            "r": self.center,
            "c": self.probabilityRegion.center,
            "W": self.probabilityRegion.region
        }.items()

    def __repr__(self):
        return self.__str__()

class RecordStateDto:

    def __init__(self, record:Record, velocity: float) -> None:
        self.record = record
        self.velocity = velocity

    record : Record
    velocity : float




lastKnownState = defaultdict(str) # RecordStateDto
samplingCoefficient = 3 # this is value initiated the best 

while True:

    unit = 0.1 # maximum length a plane can take in that moment
    s = api.get_states() 


    for state in s.states:
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
            width = abs(region.lastKnownRegion.longitude-region.center.longitude)
            region.area.width = width * samplingCoefficient ## samplingCoeffient is 
            region.area.height = height * samplingCoefficient

        record = Record(oid, location, region)
        # subprocess pipe for read by cpp
        lastKnownState[oid] = RecordStateDto(record=record, velocity=state.velocity)

            

    time.sleep(1)



