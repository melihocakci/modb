import json
from dataclasses import dataclass
import collections


@dataclass
class Jsonifier(object):
    
    def toJsonRecord(self):
        return json.dumps(self.record.dict())



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
class LocationArea:

    def __init__(self, startPoint, endPoint) -> None:
        self.startPoint = startPoint
        self.endPoint = endPoint

    startPoint : Location 
    endPoint : Location 

    def __iter__(self):
        yield self.width
        yield self.height
    
    def __repr__(self):
        return f"({self.startPoint.__dict__}, {self.endPoint.__dict__})"

    def dict(self):
        return {'pointLow': self.startPoint.__dict__, 'pointHigh': self.endPoint.__dict__}


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


# Generic dictionat to object conversion
def dict2obj(d):
     
    # checking whether object d is a
    # instance of class list
    if isinstance(d, list):
           d = [dict2obj(x) for x in d]
 
    # if d is not a instance of dict then
    # directly object is returned
    if not isinstance(d, dict):
           return d
  
    # declaring a class
    class C:
        pass
  
    # constructor of the class passed to obj
    obj = C()
  
    for k in d:
        obj.__dict__[k] = dict2obj(d[k])
  
    return obj
    
@dataclass
class Record:

    def __init__(self, oid: str,  baseLocation: Location, probabilityRegion: Region) -> None:
        self.oid = oid
        self.baseLocation = baseLocation
        self.probabilityRegion = probabilityRegion

    baseLocation : Location
    probabilityRegion : Region
    oid : int

    def json2obj(self, json :json ):
        self.oid = json["oid"]

    def dict(self):
        return {'oid': self.oid, 'baseLocation': self.baseLocation.__dict__, 'probabilityRegion': self.probabilityRegion.dict() }

    # def dict2obj(self):
    #     return 

@dataclass
class MovingObjectRecord:    

    def dict(self):
        return {'id': self.oid, 'baseLocation': self.baseLocation.__dict__, 'mbrRegion': self.mbrRegion.dict() }

    oid : int
    baseLocation : Location
    mbrRegion : LocationArea 



if __name__ == "test":

    record = Record("oid", Location(10.0,5.5), Region(Location(2.2,3.3), Rectangle(2.2,3.3)))

    loc = Location(10.0,5.5)

    s = json.dumps(record.dict())

    print(s)

