import { combineReducers } from 'redux';
import { routeReducer as router } from 'redux-simple-router';

import Immutable from 'immutable';

import counter from 'redux/modules/counter';
import pixelEditor from 'redux/modules/pixelEditor';
import layout from 'redux/modules/layout';
import settings from 'redux/modules/settings';

export default Immutable.fromJS(combineReducers({
  router,
  counter,
  pixelEditor,
  layout,
  settings,
}));
