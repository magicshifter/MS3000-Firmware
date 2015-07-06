import {noop, merge, settingsRoute} from '../../utils';
import {wifi} from '../../config.js';

export function wifiRoute(req = {}, res = {}, cb = noop) {
  settingsRoute(req, res, wifi, cb);
}

export function wifiAddRoute(req = {}, res = {}, cb = noop) {
  settingsRoute(req, res, wifi, cb);
}

export function wifiRemoveRoute(req = {}, res = {}, cb = noop) {
  settingsRoute(req, res, wifi, cb);
}

export default wifiRoute;
