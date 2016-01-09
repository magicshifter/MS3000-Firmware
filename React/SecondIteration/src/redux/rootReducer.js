import { combineReducers } from 'redux';
import { routeReducer as router } from 'redux-simple-router';

import counter from './modules/counter';
import pixelEditor from './modules/pixelEditor';
import win from './modules/window';

export default combineReducers({
  router,
  counter,
  pixelEditor,
  win,
});
