const objProto = Object.prototype;
const toStr = objProto.toString;

export const cleanType =
  t => {
    if (t === 'array') { return '[object Array]'; }

    if (t === 'nodeList' || t === 'nodelist') { return '[object ' + t + ']'; }

    if (t === 'object') { return '[object Object]'; }

    return t;
  };

export const cleanTypes =
  (types, ...addTypes) => {
    types = addTypes.concat(types).map(t => cleanType(t));

    return types.length === 1 ? types[0] : types;
  };

export const test =
  (ele, types, ...addTypes) => (
    addTypes
      .concat(types)
      .some(t => test(ele, cleanTypes(t)))
      .length > 0
  );

export const isType =
  (ele, ...types) => test(ele, types);
export const isNotType =
  (ele, ...types) => !test(ele, types);

export const isArray =
  ele => toStr.call(ele) === '[object Array]';
export const isBoolean =
  ele => typeof ele === 'boolean';
export const isDefined =
  ele => typeof ele !== 'undefined';
export const isFunction =
  ele => typeof ele === 'function';
export const isNumber =
  ele => typeof ele === 'number' && parseFloat(ele, 10) === parseFloat(ele, 10);
export const isObject =
  ele => typeof ele === 'object';
export const isString =
  ele => typeof ele === 'string';

export const isRGBAObject =
  e => isObject(e) && isNumber(e.r) && isNumber(e.g) && isNumber(e.b) && isNumber(e.a);

export const isRGBObject =
  e => isObject(e) && isNumber(e.r) && isNumber(e.g) && isNumber(e.b);

export const isColor =
  e => isRGBAObject(e) || isRGBObject(e);

export const isDate =
  ele => (ele.constructor === Date);

export const isTruthy =
  ele => !!ele;
export const isFalsy =
  ele => (!ele || isEmpty(ele));

export const isEmpty =
  ele => (!ele || !Object.keys(ele).length);

export const isNodeList =
  ele => toStr.call(ele) === '[object NodeList]';
