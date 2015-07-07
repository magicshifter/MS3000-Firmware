import {noop} from './noop';
/*
 * Function each
 * applies a function on each value in the passed object
 *
 * @param {Object} obj - Object to iterate over
 * @param {Function} fn - function to execute on each item in the objects
 * @param {String=} parentKey=false - returned to keep inheritance possible
 *
 * @returns key, value, parentKey = false) => {
 */
export function each(o = {}, f = noop) {
  let returnArray = [];
  Object.keys(o).forEach((k) => {
    returnArray.push(f(o[k], k));
  });

  return returnArray;
}
