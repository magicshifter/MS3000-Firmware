/* Function arrayify
 * Given an Object this function returns an array of key value pairs.
 * {name: 'test'} becomes [{key: name, value: 'test'}]
 *
 * @param {Object} obj - the Object to arrayify
 *
 * @returns {Array} array - The arrayified Object in {key:value} pairs.
 */
export function arrayify(obj = {}, ancestor = false) {
  let arr = [];
  Object.keys(obj).forEach(key => {
    let value = obj[key];
    if (typeof value === 'object') {
      value = arrayify(value, key);
    }

    arr.push({key, value, ancestor});
  });

  return arr;
}

export function objectify(arr = []) {
  let obj = {};
  arr
    .filter(item => item && item.key)
    .forEach((item) => {
      if (Object.prototype.toString.call(item.value) === '[object Array]') {
        item.value = objectify(item.value);
      }

      obj[item.key] = item.value;
    });

  return obj;
}
