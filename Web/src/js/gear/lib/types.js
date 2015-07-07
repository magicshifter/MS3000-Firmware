/*
 * Function is
 * test if passed value is not undefined.
 * If a type argument is passed tests if typeof v matches that type.
 *
 * @param {*} v - value to test
 * @param {String=} type=false - optional type to match typeof v against
 *
 * @returns {Boolean} - returns true if v is not undefined or does have the type
 */
export function is(v, type = false) {
  if (!type) { return typeof v !== 'undefined'; }

  return typeof v === type;
}

/*
 * Function not
 * test if passed value is undefined.
 * if type param is provided test if value is not of this type
 *
 * @param {*} v - variable to test
 * @param {String=} type=false - optional type to match typeof v against
 *
 * @returns {Boolean} - returns true if v is undefined or does not have the type
 */
export function not(v, type = false) {
  if (!type) { return typeof v === 'undefined'; }

  return typeof v !== type;
}

/*
 * Function isF
 * test if passed value is a Function.
 *
 * @param {*} v - variable to test
 */
export function isF(v) { return typeof v === 'function'; }

/*
 * Function isO
 * test if passed value is an Object.
 * Will also return true if passed a subclass of Object.
 *
 * @param {*} v - variable to test
 *
 * @returns {Boolean} - returns true if v is an object
 */
export function isO(v) { return typeof v === 'object'; }

/*
 * Function isS
 * test if passed value is a String.
 *
 * @param {*} v - variable to test
 *
 * @returns {Boolean} - returns true if v is a string
 */
export function isS(v) { return typeof v === 'string'; }

/*
 * Function isN
 * test if passed value is a Number.
 *
 * @param {*} v - variable to test
 *
 * @returns {Boolean} - returns true if v is a number
 */
export function isN(v) { return typeof v === 'number'; }

/*
 * Function isA
 * test if passed value is an Array.
 *
 * @param {*} view - variable to test
 *
 * @returns {Boolean} - returns true if v is an array
 */
export function isA(v) {
  return Object.prototype.toString.call(v) === '[object Array]';
}

/*
 * Function isView
 * test if passed value is a riot view.
 *
 * @param {*} view - variable to test
 *
 * @returns {Boolean} - returns true if v is a riot view
 */
export function isView(v) {
  return !!(v && typeof v.trigger === 'function');
}

/*
 * Function has
 * test if passed value has the passed keys
 * if type param is provided test if all value[keys] are not of this type
 *
 * @param {*} v - value to test
 * @param {keys} keys - keys to match
 * @param {Function|String=} type=false - optional type to match keys against
 *
 * @returns {Boolean} - returns true if v is undefined or does not have the type
 */
export function has(args = {keys: [], type: false}) {
  let {value, keys, type} = args;
  let tested = keys.filter((key) => {
    let exists = false;
    if (type) {
      if (isF(type)) {
        return type(value[key]);
      } else if (isS(type)) {
        return typeof value[key] === type;
      }
    } else {
      return value && isF(value.hasOwnProperty) && value.hasOwnProperty(key);
    }
  });

  return tested.length === keys.length;
}
