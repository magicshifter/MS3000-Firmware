export function lintHtml(g, conf, p) {
  let src = p.join(conf.dirs.dist, '*.html');

  return task;

  function task() {
    return g.src(src)
      .pipe(p.htmlhint());
  }
}
