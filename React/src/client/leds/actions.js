import Promise from 'bluebird';
import setToString from '../lib/settostring';
import {ValidationError} from '../lib/validation';
import {dispatch} from '../dispatcher';
import {validate} from '../validation';
import {msg} from '../intl/store';


export function toggleLed(data) {
  dispatch(toggleLed, data);
}

export function changeLed(data) {
  dispatch(changeLed, data);
}

setToString('leds', {
  toggleLed,
  changeLed,
});
