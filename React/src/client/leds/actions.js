import Promise from 'bluebird';
import setToString from '../lib/settostring';
import {dispatch} from '../dispatcher';
import {msg} from '../intl/store';

import {ledsCursor} from '../state';

import fetch from 'isomorphic-fetch';

import color from 'sc-color';

export function toggleLed(data) {
  dispatch(toggleLed, data);
}

export function changeLed(data) {
  dispatch(changeLed, data);
}

export function changeActiveColor(data) {
  updateActiveLeds(data);
  return dispatch(changeActiveColor, data);
}

export function updateActiveLeds(data) {
  ledsCursor(cursor => {
    const leds = cursor
      .get('list')
      .map(val => {
        if (val && val.get('active')) {
          return val.set('value', data);
        }

        return val;
      });

    fetchActiveLeds(leds);

    dispatch(updateActiveLeds, leds);

    return cursor.set('list', leds);
  });
}

let lastFetchFinished = true;

export function fetchActiveLeds(leds) {
  let byteString = '';

  ledsCursor(cursor => {
    const leds = cursor
      .get('list')
      .forEach(val => {
        byteString += createByteString(val);
      });

    return cursor;
  });


  if (!lastFetchFinished) {
    return;
  }
  lastFetchFinished = false;

  fetch(`http://magicshifter.local/leds?b=${btoa(byteString)}`)
  .then(
    (res) => { console.log('res', res); },
    (error, res) => { console.log('res', res, 'error', error); }
  ).then(
    () => { lastFetchFinished = true; }
  );
}

function createByteString(val = {}) {
  const col = color(val.get('value'));
  let bs = '';
  //format is bgr not rgb
  bs += String.fromCharCode(col.blue());
  bs += String.fromCharCode(col.green());
  bs += String.fromCharCode(col.red());
  bs += String.fromCharCode(0xff);

  return bs;
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
  changeActiveColor,
});
