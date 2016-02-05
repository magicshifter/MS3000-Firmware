import {createAction, handleActions} from 'redux-actions';
import Immutable from 'immutable';

import {
  currentColor, menuTextColor, materialColors,
  links, fontSize, sidebar, layout, zoom,
} from 'GLOBALS';

const {height, width} = layout;

export const defaultState = Immutable.Map({
  fontSize,
  width,
  height,
  header: {
    height: 50,
  },
  sidebar,
  currentColor,
  menuTextColor,
  materialColors,
  links,
  zoom,
});

// ------------------------------------
// Constants
// ------------------------------------

export const WINDOW_RESIZE = 'WINDOW_RESIZE';
export const SCROLL_EVENT = 'SCROLL_EVENT';

// ------------------------------------
// Actions
// ------------------------------------

export const windowResize = createAction(
  WINDOW_RESIZE,
  (value = {width: window.innerWidth, height: window.innerHeight}) => value
);

export const scrollEvent = createAction(
  SCROLL_EVENT,
  value => value
);

export const actions = {
  windowResize,
  scrollEvent,
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

  [SCROLL_EVENT]:
    (state, {payload}) => {
      const {wheelDeltaY} = payload;
      const oldZoom = state.get('zoom');

      const clampedDelta = wheelDeltaY < 0 ? -1 : 1;

      let newZoom = oldZoom + (clampedDelta * 0.1);
      newZoom = Math.min(newZoom, 3);
      newZoom = Math.max(newZoom, 0);

      return state.set('zoom', newZoom);
    },

}, defaultState);
