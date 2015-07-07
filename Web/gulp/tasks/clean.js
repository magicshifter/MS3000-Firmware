export function clean(g, conf, p) {
  let dist = p.join(conf.dirs.dist, '*');

  return task;

  function task() {
    return g.src(dist, {read: false})
      .pipe(p.rimraf(dist))
      .on('error', p.util.log);
  }
}
