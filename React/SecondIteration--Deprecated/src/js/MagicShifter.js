import React from 'react';
import ReactDom from 'react-dom';

import Root from './components/Root';

// main initialization point of the application
export default function init() {
  const appContainer = document.getElementById('app');

  if (!appContainer) {
    throw new Error('div#app not found');
  }

  ReactDom.render(<Root />, appContainer);
}
