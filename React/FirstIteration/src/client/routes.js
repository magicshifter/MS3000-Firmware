import App from './app/app.react';
import Login from './pages/login.react';
import Settings from './pages/settings.react';
import Leds from './pages/leds.react';
import Draw from './pages/draw.react';
import NotFound from './pages/notfound.react';
import React from 'react';
import {DefaultRoute, NotFoundRoute, Route} from 'react-router';

export default (
  <Route handler={App} path='/'>
    <DefaultRoute handler={Leds} name='leds' />
    <NotFoundRoute handler={NotFound} name='not-found' />r
    <Route handler={Login} name='login' />
    <Route handler={Draw} name='draw' />
    <Route handler={Settings} name='settings' />
  </Route>
);