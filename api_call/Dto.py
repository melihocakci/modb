from dataclasses import dataclass
from DataModels import Record, Rectangle, Location, Region, Jsonifier, LocationArea, MovingObjectRecord


################## IS VELOCITY CAN BE USED IN MOVING OBEJCT ############################### 
@dataclass
class RecordStateDto(Jsonifier):

    def __init__(self, record: Record, velocity: float) -> None:
        self.record = record
        self.velocity = velocity

    # def toJsonRecord(self):
    #     return json.dumps(record.dict())

    record : Record
    velocity : float


@dataclass
class Record2MovingObjectDto(Jsonifier):

    def __init__(self, record: Record) -> None:
        
        movingObjectRecord = MovingObjectRecord(record.oid,
            record.baseLocation,
            LocationArea(
                Location
                    (record.probabilityRegion.center.longitude -record.probabilityRegion.area.width/2,
                     record.probabilityRegion.center.latitude - record.probabilityRegion.area.height/2
                     ),
                Location
                    (record.probabilityRegion.center.latitude + record.probabilityRegion.area.width/2,
                     record.probabilityRegion.center.longitude + record.probabilityRegion.area.height/2
                    )
            )
        )
        self.record = movingObjectRecord


    record : MovingObjectRecord


