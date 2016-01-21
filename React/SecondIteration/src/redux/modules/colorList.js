import {createAction, handleActions} from 'redux-actions';
import Immutable from 'immutable';
import scColor from 'sc-color';

import {colorList} from 'GLOBALS';

import {isNumber, toInt} from 'utils/types';
import {rgba_toString} from 'utils/colors';

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
    (colors = []) =>
      Immutable.List(
        colors.sort(
          (colorA, colorB) =>
            scColor(rgba_toString(colorA)).hue() - scColor(rgba_toString(colorB)).hue()
        ).map(color => Immutable.Map(color))
      )
  );

export const removeColor =
  createAction(
    REMOVE_COLOR,
    colorId => colorId
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
    (state, {payload: colors}) =>
      colors,

  [REMOVE_COLOR]:
    (state, {payload: id}) =>
      isNumber(toInt(id)) && state.get(toInt(id))
        ? state.filter((_, key) => console.log(toInt(id), key, _) || toInt(id) !== key)
        : state,

}, initialState);
