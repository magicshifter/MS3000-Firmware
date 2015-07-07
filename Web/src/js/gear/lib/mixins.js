import {getJSON, postJSON, post} from './request';
import {each} from './each';
import {equal} from './equal';

/*
 * Function hide
 * hides and updates this
 * @return {Boolean} this.active - true if this is active
 */
export function isActive() { return this.active; }
/*
 * Function show
 * shows and updates this
 */
export function show() {
  this.active = true;
  this.update();
  return this;
}

/*
 * Function hide
 * hides and updates this
 */
export function hide() {
  this.active = false;
  this.update();
  return this;
}
/*
 * Function toggle
 * toggles this.active by calling show and hide
 */
export function toggle() {
  this.active ? this.hide() : this.show();
  return this;
}

/*
 * Function inherit
 * add all values specified by keys from this.parent to this
 *
 * @param {Array} keys - the keys to inherit from the parent
 */
export function inherit(args) {
  let {keys, to, from } = args;
  to = to || this;
  from = from || this.parent;
  if (to && from) {
    if (typeof keys === 'string' || typeof keys === 'number') {
      to[keys] = from[keys];
    } else if (typeof keys === 'object') {
      keys.forEach((key) => to[key] = from[key]);
    }
  }
}

export function getFormData() {
  this.loading = true;
  this.loaded = false;
  getJSON(this.formUrl, (err, xhr) => {
    if (err) { this.error = err; }

    if (!xhr.json) { this.error = 'Empty response from MagicShifter'; }

    this.loading = false;
    this.loaded = true;
    this.formData = xhr.json;
    this.update();
  });
}

export function postFormData(data) {
  console.log(this.formData, data);
  if (equal(this.formData, data)) {
    this.message = { type: 'error', text: this.__('Nothing to update.') };
    setTimeout(() => {
      this.message = null;
      this.update();
    }, 1000);

    return;
  }

  this.posting = true;
  this.posted = false;
  post(this.formUrl, data, (err, xhr) => {
    let responseText = xhr.responseText;
    this.posting = false;
    this.posted = true;
    if (responseText.length > 2 || responseText.toLowerCase() !== 'ok') {
      this.message = { type: 'error', text: xhr.responseText };
    } else {
      this.message = {type: 'success', text: this.__('Settings saved') };
    }

    setTimeout(() => {
      this.message = null;
      this.update();
    }, 1000);

    this.formData = data;
    this.update();
  });
}
