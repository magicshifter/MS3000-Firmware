import {createAction, handleActions} from 'redux-actions';
import Immutable from 'immutable';

import {currentColor, menuTextColor, materialColors, links, fontSize, sidebarWidth} from 'GLOBALS';

const height = window.innerHeight;
const width = window.innerWidth;

export const defaultState = Immutable.Map({
  fontSize,
  width,
  height,
  header: {
    height: 50,
  },
  sidebar: {
    width: sidebarWidth,
  },
  currentColor,
  menuTextColor,
  materialColors,
  links,
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
