import {waterfall} from 'async';
import {createServer} from 'http';
import url from 'url';

import routes from './routes/';
import {noop} from './utils';

export function serve(mount, cb = noop) {
  return createServer((req, res) => {
    router(mount, req, res, cb);
  });
}

function router(mount, req, res, cb = noop) {
  waterfall([
    (cb) => { cb(null, {req, res}); },

    routes,
    (args, cb) => {
      if (!args.found) {
        mount(args.req, args.res);
      }

      cb(null, args);
    },

  ], (err, args) => {
    cb(err, args);
  });
}

export default serve;
