import json
from dataclasses import dataclass
import collections

@dataclass
class Location(collections.abc.Iterable):

    def __init__(self, longitude: float, latitude: float) -> None:
        self.longitude = longitude
        self.latitude = latitude

    def __iter__(self):
        yield self.longitude
        yield self.latitude

    longitude : float
    latitude : float

    def __repr__(self):
        return f"({self.latitude}, {self.longitude})"


@dataclass
class Rectangle(collections.abc.Iterable):
    
    def __init__(self, width=None, height=None, unit=None) -> None:
        if unit == None:
            self.width = width
            self.height = height
        else:
            self.width = unit
            self.height = unit

    def __iter__(self):
        yield self.width
        yield self.height
    
    def __repr__(self):
        return f"({self.width}, {self.height})"

    width: float 
    height: float

@dataclass
class Region(collections.abc.Iterable):
    def __init__(self, location: Location, area: Rectangle ) -> None:
        self.center = location
        self.area = area
 
    def __iter__(self):
        yield self.center
        yield self.area

    def __repr__(self):
        return f"({self.center.__dict__},{self.area.__dict__})"

    def dict(self):
        return {'center': self.center.__dict__, 'area': self.area.__dict__}

    center : Location
    area : Rectangle

    
@dataclass
class Record:

    def __init__(self, oid: str,  baseLocation: Location, probabilityRegion: Region) -> None:
        self.oid = oid
        self.baseLocation = baseLocation
        self.probabilityRegion = probabilityRegion

    baseLocation : Location
    probabilityRegion : Region
    oid : int

    def dict(self):
        return {'oid': self.oid, 'baseLocaton': self.baseLocation.__dict__, 'probabilityRegion': self.probabilityRegion.dict() }


if __name__ == "test":

    record = Record("oid", Location(10.0,5.5), Region(Location(2.2,3.3), Rectangle(2.2,3.3)))

    loc = Location(10.0,5.5)

    s = json.dumps(record.dict())

    print(s)

