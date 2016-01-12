import {createAction, handleActions} from 'redux-actions';
import Immutable from 'immutable';

import {minmax} from 'utils/math';

const rows = 16;
const columns = 16;

let pixelArray = [];
for (let row = 0; row < rows; row++) {
  for (let column = 0; column < columns; column++) {
    pixelArray.push({
      row: row + 1,
      column: column + 1,
      color: {r: 0, b: 0, g: 0, a: 155},
    });
  }
}

const pixels = Immutable.List(pixelArray);

const color = Immutable.Map({r: 0, b: 0, g: 0, a: 155});

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
  (value = 1) => value
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
    const id = payload;
    const pixel = state.getIn(['pixels', id]);
    pixel.color = state.get('color').toObject();

    return state.setIn(['pixels', id], Immutable.Map(pixel));
  },

  [SET_COLOR]: (state, {payload}) => {
    const {color} = payload;
    const newState = state.set('color', color);
    return newState;
  },

  [SET_COLOR_VALUE]: (state, {payload}) => {
    const {name, value, min, max} = payload;
    const minMaxedValue = minmax(value, min, max);
    return state.setIn(['color', name], minMaxedValue);
  },

}, Immutable.Map({pixels, rows, columns, color}));
