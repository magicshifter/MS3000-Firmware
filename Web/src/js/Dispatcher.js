import riot from 'riot';
import gear from './gear';

class Dispatcher {
  constructor() {
    riot.observable(this);
    this._components = [];
  }

  emit(path, options = {}) {
    if (path) {
      this._components.forEach((c) => {
        if (gear.types.isF(c.emit)) {
          c.emit(path, options);
        }

        if (gear.types.isF(c.trigger)) {
          c.trigger(path, options);
        }
      });
    }
  }

  // Get the webcomponents registered with this Store
  get components() {
    return this._components;
  }

  // Explicitly warn if implemented and do nothing - read only.
  set components(component) {
    console.warn('Setting Store.components is not allowed.');
    return false;
  }

  // Add a component to the list of components to trigger
  // (used in tags and classes)
  enlist(component) {
    this._components.push(component);
  }

  // Remove a component from the list of components (used in tags and classes)
  delist(component) {
    let i = this._components.indexOf(component);
    if (i > -1) {
      this._components.splice(i, 1);
    }
  }
}
export default Dispatcher;
