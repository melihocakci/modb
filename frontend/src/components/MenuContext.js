import React from 'react';

export const MenuContext = React.createContext();

export const menuOption = {
    clearShowStatistics : -5,
    clearSelectParameter : -4,
    clearTakeSnapshot : -3,
    clearShowAllMbr: -2,
    clearSelectInterval: -1,
    selectInterval : 1,
    showAllMbr : 2,
    takeSnapshot : 3, // clear or take
    selectParameter : 4,
    showStatistics : 5,
}
  