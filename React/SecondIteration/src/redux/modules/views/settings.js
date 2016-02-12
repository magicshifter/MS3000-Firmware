import {createAction, handleActions} from 'redux-actions';
import Immutable from 'immutable';

import * as GLOBALS from 'GLOBALS';

const defaultValues = {protocol: GLOBALS.protocol, host: GLOBALS.host};
const initialState = Immutable.Map(defaultValues);

// ------------------------------------
// Constants
// ------------------------------------
export const SET_SETTINGS = 'SET_SETTING';

// ------------------------------------
// Actions
// ------------------------------------
export const setSettings = createAction(
  SET_SETTINGS,
  (value = defaultValues) => value
);

export const actions = {
  setSettings,
};

// ------------------------------------
// Reducer
// ------------------------------------
export default handleActions({
  [SET_SETTINGS]:
    (state, {payload}) =>
      Immutable.Map(payload),

}, initialState);
