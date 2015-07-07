import {noop} from '../utils';

export function led(req, res, cb = noop) {
  console.log(req.params);
  res.writeHead(200, {'Content-Type': 'text/plain' });
  res.end('ok');
  cb();
}

export default led;
