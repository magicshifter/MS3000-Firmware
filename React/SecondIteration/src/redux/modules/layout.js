import {createAction, handleActions} from 'redux-actions';
import Immutable from 'immutable';

import {
  currentColor, menuTextColor, materialColors,
  links, fontSize, sidebar, layout,
  zoomLevel, minZoomLevel, maxZoomLevel,
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
  zoomLevel: zoomLevel,
  minZoomLevel,
  maxZoomLevel,
});

function zoomView(state, delta) {
  const oldZoom = state.get('zoomLevel');
  const minZoom = state.get('minZoomLevel');
  const maxZoom = state.get('maxZoomLevel');

  const clampedDelta = delta < 0 ? -1 : 1;

  let newZoom = oldZoom + (clampedDelta * 0.1);
  newZoom = Math.max(newZoom, minZoom);
  newZoom = Math.min(newZoom, maxZoom);

  return newZoom;
}

// ------------------------------------
// Constants
// ------------------------------------

export const WINDOW_RESIZE = 'WINDOW_RESIZE';
export const SCROLL_EVENT = 'SCROLL_EVENT';
export const ZOOM_IN = 'ZOOM_IN';
export const ZOOM_OUT = 'ZOOM_OUT';

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

export const zoomIn = createAction(
  ZOOM_IN,
  value => value
);

export const zoomOut = createAction(
  ZOOM_OUT,
  value => value
);

export const actions = {
  windowResize,
  scrollEvent,
  zoomIn,
  zoomOut,
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
      return state.set('zoomLevel', zoomView(state, wheelDeltaY));
    },

  [ZOOM_IN]:
    state =>
      state.set('zoomLevel', state.get('zoomLevel') + 0.1),

  [ZOOM_OUT]:
    state =>
      state.set('zoomLevel', state.get('zoomLevel') - 0.1),

}, defaultState);
