import React from 'react';
import { Route, IndexRoute, Redirect } from 'react-router';

import CoreLayout from 'layouts/CoreLayout';

import AboutView from 'views/AboutView/AboutView';
import ImageView from 'views/ImageView/ImageView';
import ImageSidebarControls from 'views/ImageView/Sidebar/Controls';
import ImageSidebarFiles from 'views/ImageView/Sidebar/Files';
import ImageSidebarSettings from 'views/ImageView/Sidebar/Settings';
import sidebarMenu from 'views/ImageView/Sidebar/Menu';
import SettingsView from 'views/SettingsView/SettingsView';
import NotFoundView from 'views/NotFoundView/NotFoundView';

const components = {
  main: ImageView,
  sidebarMenu,
};

export default (
  <Route path='/' component={CoreLayout}>
    <IndexRoute
      components={{
        ...components,
        sidebar: ImageSidebarControls,
      }}
    />
    <Route
      path='/paint/files'
      components={{
        ...components,
        sidebar: ImageSidebarFiles,
      }}
    />

    <Route
      path='/paint/settings'
      components={{
        ...components,
        sidebar: ImageSidebarSettings,
      }}
    />

    <Route path='/about' component={AboutView} />
    <Route path='/settings' component={SettingsView} />
    <Route path='/404' component={NotFoundView} />
    <Redirect from='*' to='/404' />
  </Route>
);
