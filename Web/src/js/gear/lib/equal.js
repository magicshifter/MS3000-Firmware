/*
 * Function equal
 * deep compares two objects or variables.
 *
 * @param {*} val1 - first comparison variable
 * @param {*} val2 - second comparison variable
 *
 * @returns boolean - true if variables are deeply equal
 */
export function equal(val1, val2) {
  return JSON.stringify(val1) === JSON.stringify(val2);
}
