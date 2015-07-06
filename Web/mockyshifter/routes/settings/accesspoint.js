import {noop, merge, settingsRoute} from '../../utils';
import {accesspoint} from '../../config.js';

export function accesspointRoute(req = {}, res = {}, cb = noop) {
  settingsRoute(req, res, accesspoint, cb);
}

export default accesspointRoute;
