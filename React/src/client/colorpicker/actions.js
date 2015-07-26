import setToString from '../lib/settostring';
import {dispatch} from '../dispatcher';

export function addColor(data) {
  console.log('action add Color dispatching', data);
  return dispatch(addColor, data);
}

export function removeColor(data) {
  console.log('action remove Color dispatching', data);
  return dispatch(removeColor, data);
}

setToString('colorPicker', {
  addColor,
  removeColor,
});
