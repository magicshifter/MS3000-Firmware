import { combineReducers } from 'redux';
import { routeReducer as router } from 'redux-simple-router';

import Immutable from 'immutable';

import layout from 'redux/modules/layout';
import colorList from 'redux/modules/colorList';
import pixels from 'redux/modules/pixels';

import imageView from 'redux/modules/views/image';
import textView from 'redux/modules/views/text';
import settingsView from 'redux/modules/views/settings';

export default Immutable.fromJS(combineReducers({
  router,

  layout,
  colorList,
  pixels,

  imageView,
  textView,
  settingsView,
}));
