import setToString from '../lib/settostring';
import {dispatch} from '../dispatcher';

export function saveColor(data) {
  return dispatch(saveColor, data);
}

export function removeColor(data) {
  return dispatch(removeColor, data);
}

setToString('colorPicker', {
  saveColor,
  removeColor,
});
