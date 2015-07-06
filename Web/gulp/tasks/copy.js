export function copy(g, conf, p) {
  let src = [
    p.join(conf.dirs.src, 'favicon.gif'),
    p.join(conf.dirs.src, conf.dirs.img),
  ];
  let dist = conf.dirs.dist;

  return task;

  function task() {
    return g.src(src)
      .pipe(p.copy(dist, {prefix: 1}))
      .on('error', p.util.log);
  }
}
