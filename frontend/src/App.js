// import logo from './logo.svg';
// import './leaflet.css';

import { useContext, useEffect, useState, React } from 'react'

import { MenuContext, PieNameContext, menuOption } from './components/MenuContext';

import { MapContainer, TileLayer, Marker, Popup, Rectangle, useMapEvents  } from 'react-leaflet';
import DropdownLeft from './components/DropdownLeft';
// import 'https://unpkg.com/leaflet@1.7.1/dist/leaflet.css';
// import 'https://unpkg.com/react-leaflet-markercluster/dist/styles.min.css';
// import 'https://cdn.jsdelivr.net/npm/leaflet@1.7.1/dist/leaflet.css'

import L from 'leaflet';

import personIcon from './icons/person.png'
import planeIcon from './icons/dot.png'
import queryIcon from './icons/cross.png'




function App() {

  const defaultRectangle = [
    [41-5, 28-5], // Southwest coordinates
    [41+5, 28+5], // Northeast coordinates
  ]

  const [selectArea, setSelectArea] = useState(false);
  const [rectangleBounds, setRectangleBounds] = useState(defaultRectangle)
  
  const [option, setOption] = useState(-1)
  const [socketClosed, setSocketClosed] = useState(false);
  const [socketIntersectionClosed, setSocketIntersectionClosed] = useState(false);
  const [clickOnceInterval, setClickOnceInterval] = useState(true);
  const [planes, setPlanes] = useState([]);
  const [intersections, setIntersections] = useState([]);

  const [rectangleBounds_WS, setRectangleBounds_WS] = useState(defaultRectangle);


  useEffect(()=>{
    //console.log("option triggered", option)
    if ( option === menuOption.clearSelectInterval ) {
      setRectangleBounds(defaultRectangle);
      setClickOnceInterval(true);
    }
    if(option === menuOption.takeSnapshot )  {connectSocket()} 
    if (option == menuOption.queryIntersection ) {connectSocketIntersection() }

  },[option, socketClosed, socketIntersectionClosed])
  
  const mapSettings = {
    doubleClickZoom: false, // Disable double-click zoom
  };

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
        const rectangleBounds_ = JSON.parse(JSON.stringify(rectangleBounds))
        console.log(rectangleBounds_)
        // console.log('second click check : ' + rectangleBounds_WS[0][1] + "," + rectangleBounds_WS[0][0] + "," + rectangleBounds_WS[1][1] + "," + rectangleBounds_WS[1][0]);

        setRectangleBounds_WS(rectangleBounds_);
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
    //console.log("hii")
  };

  const customIcon = L.icon({
    iconUrl: personIcon,
    iconSize: [25, 25], // Customize the size of the marker icon
  });

  const planeCustomIcon = L.icon({
    iconUrl: planeIcon,
    iconSize: [15, 15], // Customize the size of the marker icon
  });

  const queryCustomIcon = L.icon({
    iconUrl: queryIcon,
    iconSize: [15, 15], // Customize the size of the marker icon
  });


  const printAllDataToMap = () => {
    //console.log("printing all data first is ");
    //console.log(planes[0]);
    //console.log( "location"  + planes[0].location[0] + planes[0].type);

    return (
      <>
        {planes.map((item, index) => {
          return item.type === 'point' ? (
            <>
              <Marker key={index}  position={[item.location[0], item.location[1]]} icon={planeCustomIcon}>
                <Popup>
                  {item.id}<br />
                </Popup>
              </Marker>
            </>
          ) : (
            null
          );
        })}
      </>
    );
  }

  const printQueriedArea = () => {
    console.log("printing queried data  is ");
    console.log(intersections[0]);
    console.log( "location"  + intersections[0].location[0] + intersections[0].type);

    return (
      <>
        {intersections.map((item, index) => {
          return item.type === 'point' ? (
            <>
              <Marker key={index}  position={[item.location[0], item.location[1]]} icon={queryCustomIcon}>
                <Popup>
                  {item.id}<br />
                </Popup>
              </Marker>
            </>
          ) : (
            null
          );
        })}
      </>
    );
  } 

  const connectSocket = () => {
    // read from websocket and read it to store data.
    const socket = new WebSocket('ws://127.0.0.1:8083');
  console.log("connect socket")
    // Connection opened
    socket.onopen = () => {
      console.log('WebSocket connection established.');
    };

    // Listen for messages
    socket.onmessage = (event) => {
      const data = event.data;
      console.log('Received data:', data);
      
      const parts = data.split(',');
      planes.push({type: parts[0], id: parts[1], location: [parts[2], parts[3]]});

      // Process the received data here
      // ...
      // storeData(data)
    };

    // Connection closed
    socket.onclose = () => {
      //console.log('WebSocket connection closed.');
      setSocketClosed(true);
      // use effect use
    };

    // while(!socketClosed) { }

    //console.log("now return");
    
    
  }

  const connectSocketIntersection = () => {
    // read from websocket and read it to store data.
    const socket = new WebSocket('ws://127.0.0.1:8083');

    // Connection opened
    socket.onopen = () => {
      console.log('WebSocket intersection connection established.');
      console.log('first Data send to ws ' + rectangleBounds_WS[0][1] + "," + rectangleBounds_WS[0][0] + "," + rectangleBounds_WS[1][1] + "," + rectangleBounds_WS[1][0]);
      socket.send(rectangleBounds_WS[0][1] + "," + rectangleBounds_WS[0][0] + "," + rectangleBounds_WS[1][1] + "," + rectangleBounds_WS[1][0]);
    };

    // Listen for messages
    socket.onmessage = (event) => {
      const data = event.data;
      console.log('Received data:', data);
      
      const parts = data.split(',');
      intersections.push({type: parts[0], id: parts[1], location: [parts[2], parts[3]]});

      // Process the received data here
      // ...
      // storeData(data)
    };

    // Connection closed
    socket.onclose = () => {
      console.log('WebSocket Intersection connection closed.');
      setSocketIntersectionClosed(true);
      // use effect use
    };

    // while(!socketClosed) { }

    //console.log("now return");
    
    
  }

  return (
    <>
      <MapContainer center={[41.0082, 28,9784]} style={{ height: '100vh' }} zoom={3} scrollWheelZoom={handleMapScroll} {...mapSettings}>
        <TileLayer
          style={{ zIndex: 10 }}
          attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
          url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
        />
        <MenuContext.Provider value={[option, setOption]}>
          <DropdownLeft style={{ zIndex: 9999 }} onClick={handleClicked}></DropdownLeft>

        </MenuContext.Provider>
        
        {(socketClosed && option !== menuOption.clearTakeSnapshot) ? (printAllDataToMap()) : null}
        {(socketIntersectionClosed && option === menuOption.queryIntersection ) ? printQueriedArea() : null } 

        <Marker position={[40.90858286754887, 29.235305786132816]} icon={customIcon}>
          <Popup>
            You are here <br />
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

// // import logo from './logo.svg';
// // import './leaflet.css';

// import { useContext, useEffect, useState, React } from 'react'

// import { MenuContext, PieNameContext, menuOption } from './components/MenuContext';

// import { MapContainer, TileLayer, Marker, Popup, Rectangle, useMapEvents  } from 'react-leaflet';
// import DropdownLeft from './components/DropdownLeft';
// // import 'https://unpkg.com/leaflet@1.7.1/dist/leaflet.css';
// // import 'https://unpkg.com/react-leaflet-markercluster/dist/styles.min.css';
// // import 'https://cdn.jsdelivr.net/npm/leaflet@1.7.1/dist/leaflet.css'

// import L from 'leaflet';

// import personIcon from './icons/person.png'
// import planeIcon from './icons/dot.png'
// import queryIcon from './icons/cross.png'




// function App() {

//   const defaultRectangle = [
//     [41-5, 28-5], // Southwest coordinates
//     [41+5, 28+5], // Northeast coordinates
//   ]

//   const [selectArea, setSelectArea] = useState(false);
//   const [rectangleBounds, setRectangleBounds] = useState(defaultRectangle)
  
//   const [option, setOption] = useState(-1)
//   const [socketClosed, setSocketClosed] = useState(false);
//   const [socketIntersectionClosed, setSocketIntersectionClosed] = useState(false);
//   const [clickOnceInterval, setClickOnceInterval] = useState(true);
//   const [planes, setPlanes] = useState([]);
//   const [intersections, setIntersections] = useState([]);

//   const [rectangleBounds_WS, setRectangleBounds_WS] = useState(defaultRectangle);


//   useEffect(()=>{
//     //console.log("option triggered", option)
//     if ( option === menuOption.clearSelectInterval ) {
//       setRectangleBounds(defaultRectangle);
//       setClickOnceInterval(true);
//     }
//   },[option, socketClosed, socketIntersectionClosed])
  
//   const mapSettings = {
//     doubleClickZoom: false, // Disable double-click zoom
//   };

//   const tileLayerStyle = {
//     zIndex: 1000, // Set the desired z-index value
//   };

//   const handleMapScroll = (event) => {
//     const map = event.target;
//     const zoom = map.getZoom();
//     const delta = event.deltaY;

//     if (delta > 0) {
//       map.setZoom(zoom - 1);
//     } else {
//       map.setZoom(zoom + 1);
//     }
//   };

//   const handleMapClick = (e) => {
//     // if(selectArea)
//     // {
//     if(option == menuOption.selectInterval) {
//       const { lat, lng } = e.latlng;
//       if(clickOnceInterval === true) {
//         console.log('Clicked first coordinates:', lat, lng);
//         rectangleBounds[0] = [lat,lng]


//         // setRectangelBound(
//         setClickOnceInterval(false)
//       } else if (clickOnceInterval === false) 
//       {
//         console.log('Clicked second coordinates:', lat, lng)
//         rectangleBounds[1] = [lat,lng]
//         const rectangleBounds_ = JSON.parse(JSON.stringify(rectangleBounds))
//         console.log(rectangleBounds_)
//         // console.log('second click check : ' + rectangleBounds_WS[0][1] + "," + rectangleBounds_WS[0][0] + "," + rectangleBounds_WS[1][1] + "," + rectangleBounds_WS[1][0]);

//         setRectangleBounds_WS(rectangleBounds_);
//         setClickOnceInterval(true)

//       }
//     }
//     // }
//   };

//   const MapClickHandler = () => {
//     useMapEvents({
//       click: handleMapClick
//     });

//     return null;
//   };

//   const handleClicked = () => {
//     //console.log("hii")
//   };

//   const customIcon = L.icon({
//     iconUrl: personIcon,
//     iconSize: [25, 25], // Customize the size of the marker icon
//   });

//   const planeCustomIcon = L.icon({
//     iconUrl: planeIcon,
//     iconSize: [15, 15], // Customize the size of the marker icon
//   });

//   const queryCustomIcon = L.icon({
//     iconUrl: queryIcon,
//     iconSize: [15, 15], // Customize the size of the marker icon
//   });


//   const printAllDataToMap = () => {
//     //console.log("printing all data first is ");
//     //console.log(planes[0]);
//     //console.log( "location"  + planes[0].location[0] + planes[0].type);

//     return (
//       <>
//         {planes.map((item, index) => {
//           return item.type === 'point' ? (
//             <>
//               <Marker key={index}  position={[item.location[0], item.location[1]]} icon={planeCustomIcon}>
//                 <Popup>
//                   {item.id}<br />
//                 </Popup>
//               </Marker>
//             </>
//           ) : (
//             null
//           );
//         })}
//       </>
//     );
//   }

//   const printQueriedArea = () => {
//     console.log("printing queried data  is ");
//     console.log(intersections[0]);
//     console.log( "location"  + intersections[0].location[0] + intersections[0].type);

//     return (
//       <>
//         {intersections.map((item, index) => {
//           return item.type === 'point' ? (
//             <>
//               <Marker key={index}  position={[item.location[0], item.location[1]]} icon={queryCustomIcon}>
//                 <Popup>
//                   {item.id}<br />
//                 </Popup>
//               </Marker>
//             </>
//           ) : (
//             null
//           );
//         })}
//       </>
//     );
//   } 

//   const connectSocket = () => {
//     // read from websocket and read it to store data.
//     const socket = new WebSocket('ws://127.0.0.1:8083');
//     console.log("i am here")
//     // Connection opened
//     socket.onopen = () => {
//       console.log('WebSocket connection established.');
//     };

//     // Listen for messages
//     socket.onmessage = (event) => {
//       const data = event.data;
//       //console.log('Received data:', data);
      
//       const parts = data.split(',');
//       planes.push({type: parts[0], id: parts[1], location: [parts[2], parts[3]]});

//       // Process the received data here
//       // ...
//       // storeData(data)
//     };

//     // Connection closed
//     socket.onclose = () => {
//       //console.log('WebSocket connection closed.');
//       setSocketClosed(true);
//       // use effect use
//     };

//     // while(!socketClosed) { }

//     //console.log("now return");
    
    
//   }

//   const connectSocketIntersection = () => {
//     // read from websocket and read it to store data.
//     const socket = new WebSocket('ws://127.0.0.1:8083');
//     console.log("connect intersection")
//     // Connection opened
//     socket.onopen = () => {
//       console.log('WebSocket intersection connection established.');
//       console.log('first Data send to ws ' + rectangleBounds_WS[0][1] + "," + rectangleBounds_WS[0][0] + "," + rectangleBounds_WS[1][1] + "," + rectangleBounds_WS[1][0]);
//       socket.send(rectangleBounds_WS[0][1] + "," + rectangleBounds_WS[0][0] + "," + rectangleBounds_WS[1][1] + "," + rectangleBounds_WS[1][0]);
//     };

//     // Listen for messages
//     socket.onmessage = (event) => {
//       const data = event.data;
//       console.log('Received data:', data);
      
//       const parts = data.split(',');
//       intersections.push({type: parts[0], id: parts[1], location: [parts[2], parts[3]]});
//       setIntersections(intersections)
//       // Process the received data here
//       // ...
//       // storeData(data)
//     };

//     // Connection closed
//     socket.onclose = () => {
//       console.log('WebSocket Intersection connection closed.');
//       setSocketIntersectionClosed(true);
//       // use effect use
//     };

//     // while(!socketClosed) { }

//     //console.log("now return");
    
    
//   }

//   return (
//     <>
//       <MapContainer center={[41.0082, 28,9784]} style={{ height: '100vh' }} zoom={3} scrollWheelZoom={handleMapScroll} {...mapSettings}>
//         <TileLayer
//           style={{ zIndex: 10 }}
//           attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
//           url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
//         />
//         <MenuContext.Provider value={[option, setOption]}>
//           <DropdownLeft style={{ zIndex: 9999 }} onClick={handleClicked}></DropdownLeft>

//         </MenuContext.Provider>
//         {(option === menuOption.takeSnapshot ) ? (connectSocket()) : null}
//         {(option == menuOption.queryIntersection ) ? (connectSocketIntersection()) : null}
//         {(socketClosed && option !== menuOption.clearTakeSnapshot) ? (printAllDataToMap()) : null}
//         {(socketIntersectionClosed && option === menuOption.queryIntersection ) ? printQueriedArea() : null } 

//         <Marker position={[40.90858286754887, 29.235305786132816]} icon={customIcon}>
//           <Popup>
//             You are here <br />
//           </Popup>
//         </Marker>
//         {(option == menuOption.selectInterval && clickOnceInterval) ? (<Rectangle bounds={rectangleBounds} color="red" fillOpacity={0.5} />) : (null)}
//         <MapClickHandler />
//       </MapContainer>


//       {/* <div className="App">
//         <header className="App-header">
//           <img src={logo} className="App-logo" alt="logo" />
//           <p>
//             Edit <code>src/App.js</code> and save to reload.
//           </p>
//           <a
//             className="App-link"
//             href="https://reactjs.org"
//             target="_blank"
//             rel="noopener noreferrer"
//           >
//             Learn React
//           </a>
//         </header>
//       </div> */}
//     </>
//   );
// }

// export default App;
