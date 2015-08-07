import State from './lib/state';

import reviveAuth from './auth/revive';
import reviveServerSettings from './settings/server/revive';
import reviveAccesspointSettings from './settings/accesspoint/revive';
import reviveUsers from './users/revive';
import reviveLeds from './leds/revive';
import reviveColorList from './colorlist/revive';

const initialState = process.env.IS_BROWSER
  ? window._appState
  : require('../server/initialstate');

export const appState = new State(initialState, function(key, value) {
  switch (key) {
    case 'auth': return reviveAuth(value);
    case 'users': return reviveUsers(value);
    case 'serverSettings': return reviveServerSettings(value);
    case 'accesspointSettings': return reviveAccesspointSettings(value);
    case 'ledsSettings': return reviveLeds(value);
    case 'colorList': return reviveColorList(value);
  }
});

export const authCursor = appState.cursor(['auth']);
export const serverSettingsCursor = appState.cursor(['serverSettings']);
export const apSettingsCursor = appState.cursor(['accesspointSettings']);
export const i18nCursor = appState.cursor(['i18n']);
export const pendingActionsCursor = appState.cursor(['pendingActions']);
export const usersCursor = appState.cursor(['users']);
export const ledsCursor = appState.cursor(['leds']);
export const colorListCursor = appState.cursor(['colorList']);
