import React from 'react';
import { Route, IndexRoute, Redirect } from 'react-router';

import CoreLayout from 'layouts/CoreLayout';

import AboutView from 'views/AboutView/AboutView';
import TextView from 'views/TextView/TextView';
import ImageView from 'views/ImageView/ImageView';
import SettingsView from 'views/SettingsView/SettingsView';
import NotFoundView from 'views/NotFoundView/NotFoundView';

export default (
  <Route path='/' component={CoreLayout}>
    <IndexRoute component={AboutView} />
    <Route path='/paint' component={ImageView} />
    <Route path='/settings' component={SettingsView} />
    <Route path='/404' component={NotFoundView} />
    <Redirect from='*' to='/404' />
  </Route>
);
