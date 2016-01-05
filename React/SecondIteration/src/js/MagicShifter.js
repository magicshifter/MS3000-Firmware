import React from 'react';
import ReactDom from 'react-dom';

import Root from './components/Root';

// main initialization point of the application
export default function init() {
  const appContainer = document.getElementById('app')

  if (!appContainer) {
    throw 'div#app not found';
    return;
  }

  ReactDom.render(
    <Root
      text='yay, this prop got passed down from MagicShifter.js'
    />,
    appContainer
  );
}
