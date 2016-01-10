import {createAction, handleActions} from 'redux-actions';
import assign from 'object-assign';

// ------------------------------------
// Constants
// ------------------------------------
export const SET_SETTINGS = 'SET_SETTING';

// ------------------------------------
// Actions
// ------------------------------------
export const setSettings = createAction(
  SET_SETTINGS,
  (value = {protocol: 'http', host: 'magicshifter.local'}) => value
);

export const actions = {
  setSettings,
};

// ------------------------------------
// Reducer
// ------------------------------------
export default handleActions({
  [SET_SETTINGS]: (state, {payload}) => assign({}, state, payload),
}, {protocol: 'http', host: 'magicshifter.local'});
