import { createAction } from 'redux-action';
import { handleActions } from 'redux-actions';

import Immutable from 'immutable';

import { isColor, isNumber } from 'utils/types';

import { rows, visibleColumns, totalColumns, defaultEditorColor } from 'GLOBALS';

const color = Immutable.Map(defaultEditorColor);

const initialState = Immutable.Map({ rows, visibleColumns, totalColumns, color });

// ------------------------------------
// Constants
// ------------------------------------

export const SET_COLOR = 'SET_COLOR';
export const SET_COLOR_VALUE = 'SET_COLOR_VALUE';
export const SET_COLUMNS = 'SET_COLUMNS';

// ------------------------------------
// Actions
// ------------------------------------

export const setColor =
  createAction(
    SET_COLOR,
    (value = { r: 0, b: 0, g: 0, a: 255 }) => value
  );

export const setColorValue =
  createAction(
    SET_COLOR_VALUE,
    (value = { name: '', value: 0, min: 0, max: 255 }) => value
  );

export const setColumns = createAction(
  SET_COLUMNS,
  (value = { name: '' }) => value
);

export const actions = {
  setColor,
  setColorValue,
  setColumns,
};

// ------------------------------------
// Reducer
// ------------------------------------

export default handleActions({

  [SET_COLOR]:
    (state, { payload: p }) => {
      const { color } = p;
      color.a = 255;

      return isColor(color)
        ? state.set('color', Immutable.Map(color))
        : state;
    },

  [SET_COLOR_VALUE]:
    (state, { payload: p }) =>
      state.setIn(['color', p.name], isNumber(p.value) ? p.value : 0),

  [SET_COLUMNS]:
    (state, { payload: p }) =>
      isNumber(parseInt(p.value, 10))
      ? state.set('visibleColumns', parseInt(p.value, 10))
      : state,

}, initialState);
