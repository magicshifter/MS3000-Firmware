import {noop} from '../utils';

export function led(req, res, cb = noop) {
  res.writeHead(200, {'Content-Type': 'text/html' });
  res.end(`
<html>
  <head>
    <title>Settings</title>
  </head>

  <body>
    <nav><ul>
      <li><a href='/settings/webserver'>webserver</a>
      <li><a href='/settings/wifi'>wifi</a>
      <li><a href='/settings/accesspoint'>accesspoint</a>
      <li><a href='/settings/hardware'>hardware</a>
  </body>
</html>
  `);
  cb();
}

export default led;
