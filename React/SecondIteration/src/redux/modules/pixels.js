import {createAction, handleActions} from 'redux-actions';
import Immutable from 'immutable';

import {isObject} from 'utils/types';
import {createImmutablePixels} from 'utils/pixels';

import {rows, totalColumns, visibleColumns, defaultLedColor} from 'GLOBALS';

const defaultColor = Immutable.Map(defaultLedColor);

const pixels = createImmutablePixels(totalColumns, visibleColumns, rows);

const initialState = Immutable.List(pixels);

// ------------------------------------
// Constants
// ------------------------------------

export const PIXEL_CLICK = 'PIXEL_CLICK';
export const SET_PIXELS = 'SET_PIXELS';

// ------------------------------------
// Actions
// ------------------------------------

export const pixelClick =
  createAction(
    PIXEL_CLICK,
    value => value
  );

export const setPixels =
  createAction(
    SET_PIXELS,
    (pixels) =>
      Immutable.List(pixels.map(
        p => Immutable.fromJS(p)
      ))
  );

export const actions = {
  pixelClick,
  setPixels,
};

// ------------------------------------
// Reducer
// ------------------------------------

export default handleActions({

  // a pixel has been clicked
  [PIXEL_CLICK]:
    (state, {payload: p}) =>
      isObject(p.color) &&
      state.setIn(
        [p.pixel.id, 'color'],
          Immutable.Map(p.pixel.color).equals(defaultColor) ||
          !Immutable.Map(p.pixel.color).equals(Immutable.Map(p.color))
            ? p.color
            : defaultColor
      ) ||
      state,

  // set all pixels
  [SET_PIXELS]:
    (state, {payload: pixels}) =>
      isObject(pixels)
        ? pixels
        : state,

}, initialState);
