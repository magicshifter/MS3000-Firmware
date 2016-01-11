import {createAction, handleActions} from 'redux-actions';
import assign from 'object-assign';

import {minmax} from 'utils/math';

const rows = 16;
const columns = 16;

let pixels = [];
for (let row = 0; row < rows; row++) {
  for (let column = 0; column < columns; column++) {
    pixels.push({
      row: row + 1,
      column: column + 1,
      color: {r: 0, b: 0, g: 0, a: 155},
    });
  }
}

const color = {r: 0, b: 0, g: 0, a: 155};

// ------------------------------------
// Constants
// ------------------------------------
export const PIXEL_CLICK = 'PIXEL_CLICK';
export const SET_COLOR = 'SET_COLOR';
export const SET_COLOR_VALUE = 'SET_COLOR_VALUE';

// ------------------------------------
// Actions
// ------------------------------------
export const pixelClick = createAction(
  PIXEL_CLICK,
  (value = {id: 1}) => value
);

export const setColor = createAction(
  SET_COLOR,
  (value = {r: 0, b: 0, g: 0, a: 155}) => value
);

export const setColorValue = createAction(
  SET_COLOR_VALUE,
  (value = {name: '', value: 0, min: 0, max: 255}) => value
);

export const actions = {
  pixelClick,
  setColor,
  setColorValue,
};

// ------------------------------------
// Reducer
// ------------------------------------
export default handleActions({
  [PIXEL_CLICK]: (state, {payload}) => {
    const {id} = payload;

    let pixels = state.pixels;
    pixels[id].color = state.color;

    delete state.pixels;

    return assign({}, state, {pixels});
  },

  [SET_COLOR]: (state, {payload}) => {
    const {color} = payload;

    delete state.color;
    return assign({}, state, {color});
  },

  [SET_COLOR_VALUE]: (state, {payload}) => {
    const {name, value, min, max} = payload;
    let {color} = state;
    color[name] = minmax(value, min, max);

    delete state.color;
    return assign({}, state, {color});
  },

}, {pixels, rows, columns, color});
