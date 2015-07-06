export function server(g, conf, p) {
  let dist = p.join(process.cwd(), conf.dirs.dist);

  return task;

  function task(done) {
    let mount = p.st({
      path: dist,
      url: '/',
      cache: false,
    });

    p.mockyShifter(mount)
      .listen(conf.port, done);
  }
}
