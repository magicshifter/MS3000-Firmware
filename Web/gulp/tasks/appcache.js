export function appcache(g, conf, p) {
  let src = p.join(conf.dirs.src, 'manifest.appcache');
  let dist = conf.dirs.dist;

  return task;

  function task() {
    return g.src(src)
      .pipe(p.replace(/\|date\|/, new Date()))
      .on('error', p.util.log)
      .pipe(g.dest(dist));
  }
}
