// import logo from './logo.svg';
// import './leaflet.css';

// import { useEffect, useState } from 'react'

import { MapContainer, TileLayer, Marker, Popup, Rectangle, useMapEvents  } from 'react-leaflet';
import DropdownLeft from './components/DropdownLeft';
// import 'https://unpkg.com/leaflet@1.7.1/dist/leaflet.css';
// import 'https://unpkg.com/react-leaflet-markercluster/dist/styles.min.css';
// import 'https://cdn.jsdelivr.net/npm/leaflet@1.7.1/dist/leaflet.css'





function App() {

  const rectangleBounds = [
    [41-5, 28-5], // Southwest coordinates
    [41+5, 28+5], // Northeast coordinates
  ];

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
    const { lat, lng } = e.latlng;
    console.log('Clicked coordinates:', lat, lng);
  };

  const MapClickHandler = () => {
    useMapEvents({
      click: handleMapClick
    });

    return null;
  };

  return (
    <>
      <MapContainer center={[41.0082, 28,9784]} style={{ height: '100vh' }} zoom={3} scrollWheelZoom={handleMapScroll} >
        <TileLayer
          style={{ zIndex: 10 }}
          attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
          url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
        />
        <DropdownLeft style={{ zIndex: 9999 }}></DropdownLeft>
        <Marker position={[41.0082, 28,9784]}>
          <Popup>
            A pretty CSS3 popup. <br /> Easily customizable.
          </Popup>
        </Marker>
        <Rectangle bounds={rectangleBounds} color="red" fillOpacity={0.5} />
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
