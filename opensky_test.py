from opensky_api import OpenSkyApi

api = OpenSkyApi(username='onrdmr2', password='425262026Asd')

s = api.get_states()

print(s)