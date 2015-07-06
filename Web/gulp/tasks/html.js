export function html(g, conf, p) {
  let src = p.join(conf.dirs.src, conf.dirs.html, conf.dirs.pages, '*.jade');
  let dist = conf.dirs.dist;

  return function task() {
    return g.src(src)
      .pipe(p.jade({
        jade: p.compileJade,
        pretty: true,
      }))
      .on('error', p.util.log)
      .pipe(g.dest(dist));
  };
}
