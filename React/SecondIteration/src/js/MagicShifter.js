import React from 'react';
import ReactDom from 'react-dom';
import { Router, Route, Link, browserHistory, IndexRoute } from 'react-router';

import Root from './components/Root';
import App from './components/App';
import PixelEditor from './components/PixelEditor/PixelEditor';
import Settings from './components/Settings/Settings';
import NoMatch from './components/NoMatch';

import globals from './GLOBALS';

// main initialization point of the application
export default function init() {
  const appContainer = document.getElementById('app')

  if (!appContainer) {
    throw 'div#app not found';
    return;
  }

  const routes = {
    path: '/',
    component: Root,
    indexRoute: { component: App },
    childRoutes: [
      { path: 'pixels', component: PixelEditor, },
      { path: 'settings', component: Settings, },
      { path: '*', component: NoMatch, },
    ],
  }

  ReactDom.render(
    <Router history={browserHistory} routes={routes}></Router>,
    appContainer
  );
}
