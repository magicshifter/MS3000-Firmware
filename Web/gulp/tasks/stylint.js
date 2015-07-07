export function stylint(g, conf, p) {
  let src = p.join(conf.dirs.src, conf.dirs.css, '*.styl');

  return task;
  function task() {
    return g.src(src)
      .pipe(p.stylint());
  }
}
