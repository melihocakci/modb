import React, { useState, useContext } from 'react';
// import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
// import { faBars, faCamera, faCloud, faBell, faAnchor, faAreaChart } from '@fortawesome/free-solid-svg-icons';
import { MenuContext, menuOption } from './MenuContext';


const DropdownLeft = () => {
  // const [isOpen, setIsOpen] = useState(false);
  const [clear, setClear] = useState(false);
  const [option, setOption] = useContext(MenuContext)

  // const toggleDropdown = () => {
  //   setIsOpen(!isOpen);
  // };

  const handleMenuClick = (e) => {
    e.stopPropagation();

    console.log(e.target.getAttribute('option'));
    let option = e.target.getAttribute('option');
    setOption(menuOption[option])
    console.log("handle menu set - " + option)
  }

  const isClearPhase = (e) => {
    console.log("are you here"+ clear)
    // e.stopPropagation();
    let clearState = !clear
    setClear(clearState);
    console.log(clearState)
    if(clearState === false) {
      let option = e.target.getAttribute('option');
      option = 'clear' + option.charAt(0).toUpperCase() + option.slice(1);
      console.log("clearMap -- " + option);
      setOption(menuOption[option])
      e.stopPropagation();
    }
  }

  return (
    <div className="center-menu">
      <label className="menu-button" htmlFor="menu-open" aria-hidden="true">
        <span className="fa fa-bars fa-2x"></span>
      </label>
      <input className="menu-open" id="menu-open" type="checkbox" aria-hidden="true" />
      <nav className="menu" role="navigation" onClick={handleMenuClick}>
        <a href="#" className="icon fa fa-camera" option="selectInterval" onClick={isClearPhase}>Camera</a>
        <a href="#" className="icon fa fa-cloud" option="showAllMbr" onClick={isClearPhase}>The Cloud</a>
        <a href="#" className="icon fa fa-bell" option="takeSnapshot" onClick={isClearPhase}>Alerts</a>
        <a href="#" className="icon fa fa-search" option="selectParameter" onClick={isClearPhase}>Shipyards</a>
        <a href="#" className="icon fa fa-area-chart" option="showStatistics" onClick={isClearPhase}>Stats</a>
      </nav>
    </div>
    // <h1>Circular dropdown menu <span>So click it already!</span></h1>
  );
};

export default DropdownLeft;
