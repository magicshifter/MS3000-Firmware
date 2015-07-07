import {noop, merge, settingsRoute} from '../../utils';
import {server} from '../../config.js';

export function serverRoute(req = {}, res = {}, cb = noop) {
  settingsRoute(req, res, server, cb);
}

export default serverRoute;
