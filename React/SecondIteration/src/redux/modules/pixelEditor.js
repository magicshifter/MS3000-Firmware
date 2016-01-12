import {createAction, handleActions} from 'redux-actions';
import Immutable from 'immutable';
import assign from 'object-assign';

import {isColor, isNumber} from 'utils/types';

const rows = 16;
const columns = 16;

let pixelArray = [];
for (let row = 0; row < rows; row++) {
  for (let column = 0; column < columns; column++) {
    const px = Immutable.Map({
      row: row + 1,
      column: column + 1,
      color: {r: 0, b: 0, g: 0, a: 155},
    });

    pixelArray.push(px);
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
export const SET_COLUMNS = 'SET_COLUMNS';

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

export const setColumns = createAction(
  SET_COLUMNS,
  (value = 1) => value
);

export const actions = {
  pixelClick,
  setColor,
  setColorValue,
  setColumns,
};

// ------------------------------------
// Reducer
// ------------------------------------
export default handleActions({
  [PIXEL_CLICK]: (state, {payload}) => {
    const id = payload;
    const pixel =
      state
        .getIn(['pixels', id])
        .set('color', state.get('color'));

    console.log('pixel clicked', pixel);

    return state.setIn(['pixels', id], pixel);
  },

  [SET_COLOR]: (state, {payload}) => {
    const {color} = payload;
    if (isColor(color)) {
      const newColor = assign({}, state.get('color'), color);
      return state.set('color', newColor);
    }

    return state;
  },

  [SET_COLOR_VALUE]: (state, {payload}) => {
    const {name} = payload;
    const value = payload.value && isNumber(payload.value)
                ? payload.value
                : 0;

    return state.setIn(['color', name], value);
  },

  [SET_COLUMNS]: (state, {payload}) => {
    const value = parseInt(payload.value, 10);

    if (isNumber(value)) {
      return state.set('columns', value);
    }

    return state;
  },

}, Immutable.Map({pixels, rows, columns, color}));
