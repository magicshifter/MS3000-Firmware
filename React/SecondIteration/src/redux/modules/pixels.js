import {createAction, handleActions} from 'redux-actions';
import Immutable from 'immutable';

import {isObject} from 'utils/types';
import {createImmutablePixels, makePixelsImmutable} from 'utils/pixels';
import {invert, lighten, darken} from 'utils/colors';

import {rows, totalColumns, visibleColumns, defaultLedColor} from 'GLOBALS';

const defaultColor = Immutable.Map(defaultLedColor);

const pixels = createImmutablePixels(totalColumns, visibleColumns, rows);

const initialState = Immutable.List(pixels);

// ------------------------------------
// Constants
// ------------------------------------

export const PIXEL_CLICK = 'PIXEL_CLICK';
export const PIXEL_HOVER = 'PIXEL_HOVER';
export const SET_PIXELS = 'SET_PIXELS';
export const INVERT = 'INVERT';
export const DARKEN = 'DARKEN';
export const LIGHTEN = 'LIGHTEN';

// ------------------------------------
// Actions
// ------------------------------------

export const pixelClick =
  createAction(
    PIXEL_CLICK,
    value => value
  );

export const pixelHover =
  createAction(
    PIXEL_HOVER,
    value => value
  );

export const setPixels =
  createAction(
    SET_PIXELS,
    pixels => makePixelsImmutable(pixels)
  );

export const invertPixels =
  createAction(
    INVERT,
    pixels => makePixelsImmutable(invert(pixels))
  );

export const darkenPixels =
  createAction(
    DARKEN,
    pixels => makePixelsImmutable(darken(pixels))
  );

export const lightenPixels =
  createAction(
    LIGHTEN,
    pixels => makePixelsImmutable(lighten(pixels))
  );

export const actions = {
  pixelClick,
  pixelHover,
  setPixels,
  invertPixels,
  darkenPixels,
  lightenPixels,
};

// ------------------------------------
// Reducer
// ------------------------------------

export default handleActions({

  // a pixel has been hovered
  [PIXEL_HOVER]:
    (state, {payload: p}) =>
      isObject(p.color)
      ? state.setIn([p.pixel.id, 'color'], p.color)
      : state,

   // a pixel has been clicked
  [PIXEL_CLICK]:
    (state, {payload: p}) =>
      isObject(p.color)
      ? state.setIn(
        [p.pixel.id, 'color'],
          Immutable.Map(p.pixel.color).equals(defaultColor) ||
          !Immutable.Map(p.pixel.color).equals(Immutable.Map(p.color))
            ? p.color
            : defaultColor
      )
      : state,

  // set all pixels
  [SET_PIXELS]:
    (state, {payload: pixels}) =>
      isObject(pixels)
        ? pixels
        : state,

  // invert all pixels
  [INVERT]:
    (state, {payload: pixels}) =>
      isObject(pixels)
        ? pixels
        : state,

  // darken all pixels
  [DARKEN]:
    (state, {payload: pixels}) =>
      isObject(pixels)
        ? pixels
        : state,

  // lighten all pixels
  [LIGHTEN]:
    (state, {payload: pixels}) =>
      isObject(pixels)
        ? pixels
        : state,

}, initialState);
