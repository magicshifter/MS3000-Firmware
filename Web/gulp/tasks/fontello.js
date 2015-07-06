export function fontello(g, conf, p) {
  let dist = p.join(conf.dirs.src, 'fontello');
  let config = p.join(process.cwd(), 'fontello.json');

  return task;
  function task() {
    return g.src(config)
      .pipe(p.fontello(config))
      .on('error', p.util.log)
      .pipe(g.dest(dist));
  }
}
