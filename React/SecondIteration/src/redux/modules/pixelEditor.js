import {createAction, handleActions} from 'redux-actions';
import Immutable from 'immutable';
import assign from 'object-assign';

import {isColor, isNumber, isObject} from 'utils/types';
import {createImmutablePixels} from 'utils/pixels';
import {immutable} from 'utils/immutable';

const rows = 16;
const visibleColumns = 16;
const totalColumns = 96;

const color = Immutable.Map({r: 0, b: 0, g: 0, a: 155});

const pixels = createImmutablePixels(totalColumns, visibleColumns, rows);

// ------------------------------------
// Constants
// ------------------------------------
export const PIXEL_CLICK = 'PIXEL_CLICK';
export const SET_COLOR = 'SET_COLOR';
export const SET_COLOR_VALUE = 'SET_COLOR_VALUE';
export const SET_COLUMNS = 'SET_COLUMNS';
export const SET_PIXELS = 'SET_PIXELS';

// ------------------------------------
// Actions
// ------------------------------------
export const pixelClick =
  createAction(
    PIXEL_CLICK,
    (value = 1) => value
  );

export const setPixels =
  createAction(
    SET_PIXELS,
    value => value
  );

export const setColor =
  createAction(
    SET_COLOR,
    (value = {r: 0, b: 0, g: 0, a: 155}) => value
  );

export const setColorValue =
  createAction(
    SET_COLOR_VALUE,
    (value = {name: '', value: 0, min: 0, max: 255}) => value
  );

export const setColumns = createAction(
  SET_COLUMNS,
  (value = 1) => value
);

export const actions = {
  pixelClick,
  setPixels,
  setColor,
  setColorValue,
  setColumns,
};

// ------------------------------------
// Reducer
// ------------------------------------
export default handleActions({
  [PIXEL_CLICK]:
    (state, {payload: id}) =>
      state.setIn(
        ['pixels', id, 'color'],
        state.get('color')),

  [SET_PIXELS]:
    (state, {payload: pixels}) =>
      // TODO
      // need to calculate and pass all pixels to this function,
      // image upload currently might not do so
      isObject(pixels)
      ? state.set('pixels', Immutable.List.of(...pixels.map(px => immutable(px))))
      : state,

  [SET_COLOR]:
    (state, {payload: p}) =>
      isColor(p.color)
      ? state.set('color', assign({}, state.get('color'), p.color))
      : state,

  [SET_COLOR_VALUE]:
    (state, {payload: p}) =>
      state.setIn(['color', p.name], isNumber(p.value) ? p.value : 0),

  [SET_COLUMNS]:
    (state, {payload: p}) =>
      isNumber(parseInt(p.value, 10))
      ? state.set('visibleColumns', parseInt(p.value, 10))
      : state,

}, Immutable.Map({pixels, rows, visibleColumns, totalColumns, color}));
