import {arrayify} from './lib/arrayify';
import {each} from './lib/each';
import {hexToRGB} from './lib/hexToRGB';
import {hasLocalStorage} from './lib/hasLocalStorage';
import {equal} from './lib/equal';
import {stopEvent} from './lib/events';
import * as mixins from './lib/mixins';
import * as request from './lib/request';
import * as types from './lib/types';

export let gear = {
  arrayify,
  each,
  hexToRGB,
  hasLocalStorage,
  equal,
  stopEvent,
  types,
  mixins,
  request,
  types,
};

export default gear;
