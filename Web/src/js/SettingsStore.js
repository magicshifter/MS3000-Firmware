import Store from './Store';
import gear from './gear';

class SettingsStore extends Store {
  constructor(externalApi) {
    super();

    this._settings = {};

    this._broadcast = externalApi.trigger;
    externalApi.enlist(this);

    this.on('settings:set', (key, value) => this.set(key, value));
  }

  set(key, value) {
    if (key && typeof value !== 'undefined') {
      this._settings[key] = value;
      this._broadcast('settings:update', this._settings);
    }
  }

  get(key) {
    if (key) {
      return this._settings[key];
    }

    return this._settings;
  }
}

export default LedStore;
