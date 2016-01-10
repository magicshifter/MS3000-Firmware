import React from 'react';
import { Route, IndexRoute, Redirect } from 'react-router';

import CoreLayout from 'layouts/CoreLayout';

import AboutView from 'views/AboutView/AboutView';
import PixelEditorView from 'views/PixelEditorView/PixelEditorView';
import NotFoundView from 'views/NotFoundView/NotFoundView';

export default (
  <Route path='/' component={CoreLayout}>
    <IndexRoute component={PixelEditorView} />
    <Route path='/about' component={AboutView} />
    <Route path='/404' component={NotFoundView} />
    <Redirect from='*' to='/404' />
  </Route>
);
