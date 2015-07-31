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

export function updateActiveLeds(data) {
  return dispatch(updateActiveLeds, data);
}

export function selectAllLeds(data) {
  dispatch(selectAllLeds, data);
}

export function deselectAllLeds(data) {
  dispatch(deselectAllLeds, data);
}

export function toggleAllLeds(data) {
  dispatch(toggleAllLeds, data);
}

export function activateLed(data) {
  dispatch(activateLed, data);
}

export function startSelection(e) {
  dispatch(startSelection, e);
}

export function stopSelection(e) {
  dispatch(stopSelection, e);
}

setToString('leds', {
  toggleLed,
  changeLed,
  updateActiveLeds,
  selectAllLeds,
  deselectAllLeds,
  toggleAllLeds,
  activateLed,
  startSelection,
  stopSelection,
});
