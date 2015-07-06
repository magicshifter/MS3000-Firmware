export function compress(g, conf, p) {
  let src = [
    p.join(conf.dirs.dist, 'index.html'),
    p.join(conf.dirs.dist, 'manifest.appcache'),
    p.join(conf.dirs.dist, 'favicon.gif'),
  ];
  let dist = conf.dirs.dist;

  return task;

  function task() {
    return g.src(src)
      .pipe(p.gzip({
        gzipOptions: {
          level: 9,
          append: true,
        },
      }))
      .on('error', p.util.log)
      .pipe(g.dest(dist));
  }
}
