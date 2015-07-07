import {noop, merge, settingsRoute} from '../../utils';
import {hardware} from '../../config.js';

export function hardwareRoute(req = {}, res = {}, cb = noop) {
  settingsRoute(req, res, hardware, cb);
}

export default hardwareRoute;
