export function noop() {};

export function merge(obj1 = {}, obj2 = {}) {
  Object.keys(obj2).forEach((key) => {
    obj1[key] = obj2[key];
  });

  return obj1;
}

export function settingsRoute(req, res, data, cb = noop) {
  if (req.method === 'POST' && req.data) {
    data = merge(data, req.data);
    res.writeHead(200, {'Content-Type': 'text/plain'});
    res.end('ok');
    return cb();
  }

  res.writeHead(200, {'Content-Type': 'application/json'});
  res.end(JSON.stringify(data));
  cb();
}
