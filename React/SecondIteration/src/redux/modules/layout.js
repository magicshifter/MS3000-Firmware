import {createAction, handleActions} from 'redux-actions';
import Immutable from 'immutable';

const height = window.innerHeight;
const width = window.innerWidth;

export const defaultState = Immutable.Map({
  fontSize: 15,
  width,
  height,
  header: {
    height: 50,
  },
  sidebar: {
    width: 200,
  },
});

// ------------------------------------
// Constants
// ------------------------------------
export const WINDOW_RESIZE = 'WINDOW_RESIZE';

// ------------------------------------
// Actions
// ------------------------------------
export const windowResize = createAction(
  WINDOW_RESIZE,
  (value = {width: window.innerWidth, height: window.innerHeight}) => value
);

export const actions = {
  windowResize,
};

// ------------------------------------
// Reducer
// ------------------------------------
export default handleActions({
  [WINDOW_RESIZE]:
    (state, {payload}) =>
      state
        .set('width', window.innerWidth)
        .set('height', window.innerHeight),

}, defaultState);
