import React from 'react';
import {Route, IndexRoute, Redirect} from 'react-router';

import CoreLayout from 'layouts/CoreLayout';

import AboutView from 'views/AboutView/AboutView';

import ImageSidebarControls from 'views/ImageView/Sidebar/Controls';
import ImageSidebarImages from 'views/ImageView/Sidebar/Images';
import ImageSidebarText from 'views/ImageView/Sidebar/Text';
import ImageSidebarFilters from 'views/ImageView/Sidebar/Filters';
import ImageSidebarSettings from 'views/ImageView/Sidebar/Settings';
import SidebarFooter from 'views/ImageView/Sidebar/Footer';

import SettingsView from 'views/SettingsView/SettingsView';
import NotFoundView from 'views/NotFoundView/NotFoundView';

const components = {
  footer: SidebarFooter,
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
      path='/paint/images'
      components={{
        ...components,
        sidebar: ImageSidebarImages,
      }}
    />

    <Route
      path='/paint/font'
      components={{
        ...components,
        sidebar: ImageSidebarText,
      }}
    />

    <Route
      path='/paint/effects'
      components={{
        ...components,
        sidebar: ImageSidebarFilters,
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
