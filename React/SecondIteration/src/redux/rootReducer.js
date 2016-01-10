import { combineReducers } from 'redux';
import { routeReducer as router } from 'redux-simple-router';

import counter from 'redux/modules/counter';
import pixelEditor from 'redux/modules/pixelEditor';
import layout from 'redux/modules/layout';
import settings from 'redux/modules/settings';

export default combineReducers({
  router,
  counter,
  pixelEditor,
  layout,
  settings,
});
