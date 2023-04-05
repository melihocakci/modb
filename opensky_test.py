from opensky_api import OpenSkyApi
from collections import defaultdict

import json
import time

api = OpenSkyApi(username='onrdmr2', password='425262026Asd')


global unitArea # now it is global



class Location:

    def __init__(self, longitude: float, latitude: float) -> None:
        self.longitute = longitude
        self.latitude = latitude

    longitute : float
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
    def __init__(self, location: Location, region: Rectangle ) -> None:
        self.center = location
        self.region = region
 
    center : Location
    region : Rectangle

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
samplingCoefficient = 3

while True:

    unitArea = 0.1
    s = api.get_states()


    for state in s.states:
        longitude = state.longitude
        latitute = state.latitude

        location = Location(longitude=longitude, latitude=latitute)

        oid = state.icao24
        region = Region(location=location, region=Rectangle(unit=unitArea))
        record = Record(oid, location, region)
        lastKnownRegion = None

        if(lastKnownState.get(oid) != None):
            lastKnownRegion = lastKnownState[oid].record.baseLocation

        lastKnownState[oid] = RecordStateDto(record=record, velocity=state.velocity)

        if(lastKnownRegion != None):
            ## region value will be created
            break

    time.sleep(1)



