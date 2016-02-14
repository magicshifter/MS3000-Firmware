import {createAction} from 'redux-action';
import {handleActions} from 'redux-actions';
import Immutable from 'immutable';
import localforage from 'localforage';

import * as GLOBALS from 'GLOBALS';

const defaultValues = {
  protocol: GLOBALS.protocol,
  host: GLOBALS.host,
  syslogIp: GLOBALS.syslogIp,
  ssid: GLOBALS.ssid,
};

const initialState = Immutable.Map(defaultValues);

// ------------------------------------
// Constants
// ------------------------------------
export const SET_SETTINGS = 'SET_SETTING';
export const LOAD_SETTINGS = 'LOAD_SETTINGS';
export const SAVE_SETTINGS = 'SAVE_SETTINGS';

// ------------------------------------
// Actions
// ------------------------------------
export const setSettings = createAction(
  SET_SETTINGS,
  (value = defaultValues) => value
);

export const loadSettings = createAction(
  LOAD_SETTINGS,
  (value = defaultValues) => value
);

export const saveSettings = createAction(
  SAVE_SETTINGS,
  (value = defaultValues) => value
);

export const actions = {
  setSettings,
  loadSettings,
  saveSettings,
};

// ------------------------------------
// Reducer
// ------------------------------------
export default handleActions({
  [SET_SETTINGS]:
    (state, {payload}) =>
      Immutable.Map(payload),

  [SAVE_SETTINGS]:
    (state, {payload}) =>
      localforage.setItem('settings', payload),

  [LOAD_SETTINGS]:
    (state, {payload}) =>
      localforage.getItem('settings'),

}, initialState);
