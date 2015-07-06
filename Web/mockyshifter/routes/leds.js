import {noop} from '../utils';
import {hardware} from '../config';

let l = [];
for (let i = 0; i < hardware.leds.count; i++) {
  l.push([0, 0, 0, 0]);
}

export function leds(req, res, cb = noop) {
  if (req.method === 'POST') {
    res.writeHead(200, {'Content-Type': 'text/plain' });
    res.end('ok');
  } else {
    res.writeHead(404, {'Content-Type': 'application/json'});
    res.end(JSON.stringify(l));
  }
  cb();
}
export default leds;
