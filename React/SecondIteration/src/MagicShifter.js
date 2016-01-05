import React from 'react';
import ReactDom from 'react-dom';

import ExampleComponent from './components/Example';

// main initialization point of the application
export default function init() {
  const appContainer = document.getElementById('app')

  if (!appContainer) {
    throw 'div#app not found';
    return;
  }

  ReactDom.render(
    <ExampleComponent
      text='yay, this prop got passed down'
    />,
    appContainer
  );
}
