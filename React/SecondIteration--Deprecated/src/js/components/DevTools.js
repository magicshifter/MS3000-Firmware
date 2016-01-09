import React, {Component} from 'react';
import { createDevTools } from 'redux-devtools';
 
import LogMonitor from 'redux-devtools-log-monitor';
import DockMonitor from 'redux-devtools-dock-monitor';

// createDevTools takes a monitor and produces a DevTools component 
export default createDevTools(
  <DockMonitor
    defaultPosition='bottom'
    toggleVisibilityKey='ctrl-h'
    changePositionKey='ctrl-q'
  >
    <LogMonitor
      theme='tomorrow'
    />
  </DockMonitor>
);
