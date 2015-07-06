import riot from 'riot';
import gear from './gear';

class Store {
  constructor() {
    riot.observable(this);
    this._components = [];

    this.externalApi = {
      trigger: this.trigger.bind(this),
      enlist: this.enlist.bind(this),
    };
  }

  // Get the webcomponents registered with this Store
  get components() {
    return this._components;
  }

  // Explicitly warn and do nothing - read only.
  set components(component) {
    console.warn('Setting Store.components is not allowed.');
    return false;
  }

  emit(...args) {
    gear.each(this._components, (component) => {
      if (gear.types.isF(component.emit)) {
        component.emit(...args);
      } else if (gear.types.isF(component.trigger)) {
        component.trigger(...args);
      }
    });
  }

  // Add a component to the list of components to trigger
  enlist(component) {
    if (this._components.indexOf(component) === -1) {
      this._components.push(component);
    }
  }

  // Remove a component from the list of components (used in tags and classes)
  delist(component) {
    let i = this._components.indexOf(component);
    if (i > -1) {
      this._components.splice(i, 1);
    }
  }
}
export default Store;
