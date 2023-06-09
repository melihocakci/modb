import React, { useState, useContext } from 'react';
// import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
// import { faBars, faCamera, faCloud, faBell, faAnchor, faAreaChart } from '@fortawesome/free-solid-svg-icons';
import { MenuContext, menuOption } from './MenuContext';


const DropdownLeft = () => {
  const [isOpen, setIsOpen] = useState(false);

  const [option, setOption] = useContext(MenuContext)

  // const toggleDropdown = () => {
  //   setIsOpen(!isOpen);
  // };

  const handleMenuClick = (e) => {
    console.log(e.target.getAttribute('option'));
    let option = e.target.getAttribute('option');
    setOption(menuOption[option])
  }

  return (
    <div className="center-menu">
      <label className="menu-button" htmlFor="menu-open" aria-hidden="true">
        <span className="fa fa-bars fa-2x"></span>
      </label>
      <input className="menu-open" id="menu-open" type="checkbox" aria-hidden="true" />
      <nav className="menu" role="navigation" onClick={handleMenuClick}>
        <a href="#" className="icon fa fa-camera" option="selectInterval">Camera</a>
        <a href="#" className="icon fa fa-cloud" option="showAllMbr">The Cloud</a>
        <a href="#" className="icon fa fa-bell" option="clearOrTakeSnapshot">Alerts</a>
        <a href="#" className="icon fa fa-anchor" option="selectParameter">Shipyards</a>
        <a href="#" className="icon fa fa-area-chart" option="showStatistics">Stats</a>
      </nav>
    </div>
    // <h1>Circular dropdown menu <span>So click it already!</span></h1>
  );
};

export default DropdownLeft;
