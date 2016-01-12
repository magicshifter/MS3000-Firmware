import {createAction, handleActions} from 'redux-actions';
import Immutable from 'immutable';

// ------------------------------------
// Constants
// ------------------------------------
export const WINDOW_RESIZE = 'WINDOW_RESIZE';

// ------------------------------------
// Actions
// ------------------------------------
export const windowResize = createAction(
  WINDOW_RESIZE,
  (value = Immutable.Map({width: window.innerWidth, height: window.innerHeight})) => value
);

export const actions = {
  windowResize,
};

// ------------------------------------
// Reducer
// ------------------------------------
export default handleActions({
  [WINDOW_RESIZE]: (state, {payload}) => state.update(payload),
}, Immutable.Map({width: window.innerWidth, height: window.innerHeight}));
