import riot from 'riot';
import gear from './gear';
import Dispatcher from './Dispatcher';

import Router from './Router';
import Log from './Log';
import I18N from './I18N';
import config from '../config.js';
import LedStore from './LedStore';

let each = gear.each;

export class HQ extends Dispatcher {
  constructor() {
    super();
    this.config = config;

    this.externalApi = {
      emit: this.emit.bind(this),
      enlist: this.enlist.bind(this),
      delist: this.delist.bind(this),
      config: this.config,
    };

    this._i18n = new I18N(this.externalApi);
    this._router = new Router(this.externalApi);
    this._log = new Log(this.externalApi);
    this._ledStore = new LedStore(this.externalApi);

    this.createMixins();

    riot.mount('*');
    this.initialRedirect();
  }

  // Collect functions to expose to tags via riot.mixin
  createMixins() {
    riot.mixin('hq', { hq: this.externalApi });

    riot.mixin('router', { router: this._router });
    riot.mixin('ledStore', { ledStore: this._ledStore });
    riot.mixin('i18n',   { __: this._i18n.__ });
    riot.mixin('log',    {log: this._log});
    riot.mixin('gear',   gear);
    riot.mixin('request',   gear.request);
    riot.mixin('stopEvent',   { stopEvent: gear.stopEvent });
    riot.mixin('each',   { each: gear.each });
    riot.mixin('equal',   { equal: gear.equal });
    riot.mixin('noop',   { noop: gear.noop });
    riot.mixin('hexToRGB',   { hexToRGB: gear.hexToRGB });
    riot.mixin('mixins', gear.mixins);
    riot.mixin('types',   gear.types);
    riot.mixin('hasLocalStorage', {hasLocalStorage: gear.hasLocalStorage});
    riot.mixin('ledStore', {ledStore: this._ledStore});
    riot.mixin('broadcast', {broadcast: this.emit.bind(this)});
  }

  initialRedirect() {
    let redirectRoute = window.location.hash.replace('#', '');
    if (!redirectRoute) {
      redirectRoute = this.config.homepage || 'leds';
    }

    this.emit('Router:change', redirectRoute);
  }
}

export default HQ;
