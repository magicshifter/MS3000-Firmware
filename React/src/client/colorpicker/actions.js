import setToString from '../lib/settostring';
import {dispatch} from '../dispatcher';

export function saveColor(data) {
  console.log('action save Color dispatching', data);
  return dispatch(saveColor, data);
}

export function removeColor(data) {
  console.log('action remove Color dispatching', data);
  return dispatch(removeColor, data);
}

setToString('colorPicker', {
  saveColor,
  removeColor,
});
