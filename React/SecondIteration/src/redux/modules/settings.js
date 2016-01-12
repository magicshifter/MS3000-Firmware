import {createAction, handleActions} from 'redux-actions';
import Immutable from 'immutable';

// ------------------------------------
// Constants
// ------------------------------------
export const SET_SETTINGS = 'SET_SETTING';

// ------------------------------------
// Actions
// ------------------------------------
export const setSettings = createAction(
  SET_SETTINGS,
  (value = Immutable.Map({protocol: 'http', host: 'magicshifter.local'})) => value
);

export const actions = {
  setSettings,
};

// ------------------------------------
// Reducer
// ------------------------------------
export default handleActions({
  [SET_SETTINGS]: (state, {payload}) => state.set(payload),
}, Immutable.Map({protocol: 'http', host: 'magicshifter.local'}));
