export const SET_COLOR_VALUE = 'pixelEditor/color/value/set';
export const SET_COLOR = 'pixelEditor/color/set';

export function setColorValue() {
  return {
    type: SET_COLOR_VALUE,
  };
}

export function setColor() {
  return {
    type: SET_COLOR,
  };
}

export default {
  SET_COLOR_VALUE,
  SET_COLOR,
  setColorValue,
  setColor,
};
