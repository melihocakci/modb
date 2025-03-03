import React from 'react';

export const MenuContext = React.createContext();

export const menuOption = { // change need DropdownLeft
    clearShowStatistics : -5,
    // clearQueryIntersection : -4,
    clearTakeSnapshot : -3,
    clearShowAllMbr: -2,
    clearSelectInterval: -1,
    selectInterval : 1,
    showAllMbr : 2,
    takeSnapshot : 3, // clear or take
    queryIntersection : 4,
    showStatistics : 5,
}
  
