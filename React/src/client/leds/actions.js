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

export function changeBrightness(data) {
  const brightness = data.target.value;

  ledsCursor(cursor => {
    let leds = cursor.get('list');
    fetchLeds(leds, brightness);

    return cursor.set('brightness', brightness);
  });

  return dispatch(changeBrightness, data);
}

export function changeActiveColor(data) {
  updateActiveLeds(data, true);
  return dispatch(changeActiveColor, data);
}

export function updateActiveLeds(data) {
  ledsCursor(cursor => {
    const brightness = cursor.get('brightness');
    const leds = cursor
      .get('list')
      .map(val => {
        if (val && val.get('active')) {
          return val.set('value', data);
        }

        return val;
      });

    fetchLeds(leds, brightness);

    dispatch(updateActiveLeds, leds);

    return cursor.set('list', leds);
  });
}

let lastFetchFinished = true;

export function fetchLeds(leds) {
  let fetcher;

  ledsCursor(cursor => {
    let byteString = '';
    const brightness = cursor.get('brightness');

    cursor
      .get('list')
      .forEach(key => {
        const val = leds[key];
        byteString += createByteString(val, brightness);
      });

    if (!lastFetchFinished) {
      return cursor;
    }

    lastFetchFinished = false;

    fetcher = fetch(`http://magicshifter.local/leds?b=${btoa(byteString)}`)
    .then(
      (res) => { console.log('res', res); },
      (error, res) => { console.log('res', res, 'error', error); }
    ).then(
      () => { lastFetchFinished = true; }
    );

    return cursor;
  });

  if (fetcher && typeof fetcher.then === 'function') {
    dispatch(fetchLeds, fetcher);
  }
}

function createByteString(val = false, brightness = 120) {
  if (!val) { return ''; }
  console.log(val);

  const col = color(val.get('value'));
  let bs = '';
  //format is bgr not rgb
  bs += String.fromCharCode(col.blue());
  bs += String.fromCharCode(col.green());
  bs += String.fromCharCode(col.red());
  bs += String.fromCharCode(brightness);

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
  changeBrightness,
  fetchLeds,
});
