import React, { useState } from 'react';
// import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
// import { faBars, faCamera, faCloud, faBell, faAnchor, faAreaChart } from '@fortawesome/free-solid-svg-icons';

const DropdownLeft = () => {
  const [isOpen, setIsOpen] = useState(false);

  const toggleDropdown = () => {
    setIsOpen(!isOpen);
  };

  return (
    <div className="center-menu">
      <label className="menu-button" htmlFor="menu-open" aria-hidden="true">
        <span className="fa fa-bars fa-2x"></span>
      </label>
      <input className="menu-open" id="menu-open" type="checkbox" aria-hidden="true" />
      <nav className="menu" role="navigation">
        <a href="#" className="icon fa fa-camera">Camera</a>
        <a href="#" className="icon fa fa-cloud">The Cloud</a>
        <a href="#" className="icon fa fa-bell">Alerts</a>
        <a href="#" className="icon fa fa-anchor">Shipyards</a>
        <a href="#" className="icon fa fa-area-chart">Stats</a>
      </nav>
    </div>
    // <h1>Circular dropdown menu <span>So click it already!</span></h1>
  );
};

export default DropdownLeft;
