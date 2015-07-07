import Store from './Store';
import riot from 'riot';
import gear from './gear';

class LedStore extends Store {
  constructor(hq) {
    super();
    riot.observable(this);
    hq.enlist(this);

    this._leds = [];
    this._broadcast = hq.emit.bind(hq);
    this.getLeds();

    this.on('led:set', this.set.bind(this));
    this.on('led:toggle', (id) => this.toggle(id));

    this.on('leds:set', (value) => this.setAll(value));
    this.on('leds:activate', () => this.activateAll());
    this.on('leds:deactivate', () => this.deactivateAll());
  }

  isLed(id = false) {
    console.log('id', id);
    return (this._leds[id] && gear.types.is(this._leds[id].id));
  }

  set(id = false, values = {}) {
    if (this.isLed(id)) {
      let led = this._leds[id];
      gear.each(values, (val, key) => {
        led[key] = val;
      });

      this._leds[id].value = value;
      this._broadcast(`led:value:${id}`, value);
    }
  }

  toggle(id) {
    if (this.isLed(id)) {
      this.deactivateAll();
      this._leds[id].active = !this._leds[id].active;
      this._broadcast(`led:activity:${id}`, this._leds[id].active);
    }
  }

  setAll(value = false) {
    console.log('set all with value', value);
    if (value) {
      gear.each(this._leds, (led) => this.set(led.id, value));
    }
  }

  activate(id) {
    if (this.isLed(id)) {
      this._leds[id].active = true;
      this._broadcast(`led:toggle:${id}`, true);
    }
  }

  deactivate(id) {
    if (this.isLed(id)) {
      this._leds[id].active = false;
      this._broadcast(`led:toggle:${id}`, false);
    }
  }

  activateAll() {
    this._leds.forEach((led) => this.activate(led.id));
  }

  deactivateAll() {
    this._leds.forEach((led) => this.deactivate(led.id));
  }

  getLeds() {
    gear.request.getJSON('/info/hardware', (err, xhr) => {
      if (xhr.json) {
        for (let i = 0; i < xhr.json.leds.count; i++) {
          this._leds.push({
            id: i,
            value: '#000000',
            active: false,
          });
        }
      }

      this._broadcast('leds:init', this._leds);
    });
  }

  postLeds(leds) {
    if (typeof leds === 'object') {
      this._leds = leds;
      leds = JSON.stringify(leds);
    }

    gear.request.postJSON('/leds', leds, (err, xhr) => {
      this._broadcast('leds:update', leds);
    });
  }
}

export default LedStore;
