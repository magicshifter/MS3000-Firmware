import {noop} from '../utils';

let allowedModes = [
  'ball',
  'pov',
];

function modeRoute(req, res = {}, cb = noop) {
  if (res.params && res.params.query && res.params.query.mode) {
    let mode = res.params.query.mode;
    if (allowedModes.indexOf(mode) > -1) {
      let set = settings.get();
      set.mode = mode;
      settings.set(set);
    }
    cb();
  }
}
