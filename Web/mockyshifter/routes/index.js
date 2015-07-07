import led from './led';
import leds from './leds';
import settings from './settings';
import mode from './mode';
import hardware from './settings/hardware';
import accesspoint from './settings/accesspoint';
import {wifiRoute, wifiAddRoute, wifiRemoveRoute} from './settings/wifi';
import server from './settings/server';
import url from 'url';
import {noop} from '../utils';
import qs from 'querystring';

let routes = {
  '/leds': leds,
  '/led': led,
  '/mode': mode,
  '/settings/wifi': wifiRoute,
  '/settings/wifi/add': wifiAddRoute,
  '/settings/wifi/remove': wifiRemoveRoute,
  '/settings/server': server,
  '/settings/ap': accesspoint,
  '/info/hardware': hardware,
  '/settings': settings,
};

export function handleRoutes(args, cb = noop) {
  if (!args || !args.req || !args.res) { return cb(null, true); }

  args.req.params = url.parse(args.req.url);

  if (args.req.method === 'POST') {
    let requestBody = '';
    args.req.on('data', function(data) {
      requestBody += data;
      if (requestBody.length > 1e6) {
        console.log('request too large');
        args.res.end(413, 'Request body too large');
      }
    });

    args.req.on('end', function() {
      args.req.data = qs.parse(requestBody);
      console.log('routing with data', args.req.data);
      route(args, cb);
    });
  } else {
    route(args, cb);
  }
}

function route(args, cb) {
  let req = args.req;
  let url = req.url;
  let res = args.res;
  Object.keys(routes)
    .filter(k => url.indexOf(k) === 0 && typeof routes[k] === 'function')
    .forEach((key) => {
      args.found = true;
      routes[key](req, res);
    });

  cb(null, args);
}

export default handleRoutes;
