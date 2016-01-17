import {createAction, handleActions} from 'redux-actions';
import Immutable from 'immutable';

import {colorList} from 'GLOBALS';

import {isColor, isNumber, toInt} from 'utils/types';

const initialState = Immutable.List(colorList.map(color => Immutable.Map(color)));

// ------------------------------------
// Constants
// ------------------------------------

export const ADD_COLOR = 'ADD_COLOR';
export const REMOVE_COLOR = 'REMOVE_COLOR';

// ------------------------------------
// Actions
// ------------------------------------

export const addColor =
  createAction(
    ADD_COLOR,
    (value = {r: 0, g: 0, b: 0, a: 255}) => Immutable.Map(value)
  );

export const removeColor =
  createAction(
    REMOVE_COLOR,
    value => value
  );

export const actions = {
  addColor,
  removeColor,
};

// ------------------------------------
// Reducer
// ------------------------------------

export default handleActions({

  [ADD_COLOR]:
    (state, {payload: color}) =>
      isColor(color.toObject()) && !state.some(c => c.equals(color))
        ? state.push(color)
        : state,

  [REMOVE_COLOR]:
    (state, {payload: id}) =>
      isNumber(toInt(id)) && state.get(toInt(id))
        ? state.filter((_, key) => console.log(toInt(id), key, _) || toInt(id) !== key)
        : state,

}, initialState);
