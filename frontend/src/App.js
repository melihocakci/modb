// import logo from './logo.svg';
// import './leaflet.css';

import { useContext, useEffect, useState } from 'react'

import { MenuContext, PieNameContext, menuOption } from './components/MenuContext';

import { MapContainer, TileLayer, Marker, Popup, Rectangle, useMapEvents  } from 'react-leaflet';
import DropdownLeft from './components/DropdownLeft';
// import 'https://unpkg.com/leaflet@1.7.1/dist/leaflet.css';
// import 'https://unpkg.com/react-leaflet-markercluster/dist/styles.min.css';
// import 'https://cdn.jsdelivr.net/npm/leaflet@1.7.1/dist/leaflet.css'

import L from 'leaflet';

import personIcon from './icons/person.png'



function App() {

  const [ selectArea, setSelectArea ] = useState(false);
  const [ rectangleBounds, setRectangelBound ] = useState([
    [41-5, 28-5], // Southwest coordinates
    [41+5, 28+5], // Northeast coordinates
  ])
  
  const [option, setOption] = useState(-1)
  const [clickOnceInterval, setClickOnceInterval] = useState(true);
  

  useEffect(()=>{
    console.log("option triggered", option)
  },[option])
  

  const tileLayerStyle = {
    zIndex: 1000, // Set the desired z-index value
  };

  const handleMapScroll = (event) => {
    const map = event.target;
    const zoom = map.getZoom();
    const delta = event.deltaY;

    if (delta > 0) {
      map.setZoom(zoom - 1);
    } else {
      map.setZoom(zoom + 1);
    }
  };

  const handleMapClick = (e) => {
    // if(selectArea)
    // {
    if(option == menuOption.selectInterval) {
      const { lat, lng } = e.latlng;
      if(clickOnceInterval === true) {
        console.log('Clicked first coordinates:', lat, lng);
        rectangleBounds[0] = [lat,lng]
        // setRectangelBound(
        setClickOnceInterval(false)
      } else if (clickOnceInterval === false) 
      {
        console.log('Clicked second coordinates:', lat, lng)
        rectangleBounds[1] = [lat,lng]
        setClickOnceInterval(true)

      }
    }
    // }
  };

  const MapClickHandler = () => {
    useMapEvents({
      click: handleMapClick
    });

    return null;
  };

  const handleClicked = () => {
    console.log("hii")
  };

  const customIcon = L.icon({
    iconUrl: personIcon,
    iconSize: [25, 25], // Customize the size of the marker icon
  });


  const printAllDataToMap = ()=> {
    
    return (
      <>
      
      </>
    )
  }

  return (
    <>
      <MapContainer center={[41.0082, 28,9784]} style={{ height: '100vh' }} zoom={3} scrollWheelZoom={handleMapScroll} >
        <TileLayer
          style={{ zIndex: 10 }}
          attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
          url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
        />
        <MenuContext.Provider value={[option, setOption]}>
          <DropdownLeft style={{ zIndex: 9999 }} onClick={handleClicked}></DropdownLeft>

        </MenuContext.Provider>
        {(option === menuOption.clearOrTakeSnapshot) ? (printAllDataToMap) : null}
        <Marker position={[40.90858286754887, 29.235305786132816]} icon={customIcon}>
          <Popup>
            You are here <br />.
          </Popup>
        </Marker>
        {(option == menuOption.selectInterval && clickOnceInterval) ? (<Rectangle bounds={rectangleBounds} color="red" fillOpacity={0.5} />) : (null)}
        <MapClickHandler />
      </MapContainer>


      {/* <div className="App">
        <header className="App-header">
          <img src={logo} className="App-logo" alt="logo" />
          <p>
            Edit <code>src/App.js</code> and save to reload.
          </p>
          <a
            className="App-link"
            href="https://reactjs.org"
            target="_blank"
            rel="noopener noreferrer"
          >
            Learn React
          </a>
        </header>
      </div> */}
    </>
  );
}

export default App;
