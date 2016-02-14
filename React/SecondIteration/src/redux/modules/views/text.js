import { createAction } from 'redux-action';
import { handleActions } from 'redux-actions';
import Immutable from 'immutable';

import { text, fontId, fonts, textColor } from 'GLOBALS';

import { toInt } from 'utils/types';

const defaultValues = {
  text,
  fontId,
  fonts: Immutable.List(fonts),
  textColor: Immutable.Map(textColor),
};
const initialState = Immutable.Map(defaultValues);

// ------------------------------------
// Constants
// ------------------------------------
export const SET_TEXT = 'SET_TEXT';
export const SET_FONT = 'SET_FONT';
export const SET_COLOR = 'SET_COLOR';

// ------------------------------------
// Actions
// ------------------------------------
export const setText = createAction(
  SET_TEXT,
  value => value
);

export const setFont = createAction(
  SET_FONT,
  value => toInt(value)
);

export const setColor = createAction(
  SET_COLOR,
  value => value
);

export const actions = {
  setText,
  setFont,
  setColor,
};

// ------------------------------------
// Reducer
// ------------------------------------
export default handleActions({
  [SET_TEXT]: (state, { payload }) => state.set('text', payload),
  [SET_FONT]: (state, { payload }) => console.log(payload) || state.set('fontId', payload),
  [SET_COLOR]: (state, { payload: { name, value }}) => state.setIn(['textColor', name], value),

}, initialState);
