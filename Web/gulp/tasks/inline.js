export function inline(g, conf, p) {
  let src = p.join(conf.dirs.dist, 'home.html');
  let newName = 'index.html';
  let imgSrc = p.join(conf.dirs.dist, conf.dirs.img);
  let dist = conf.dirs.dist;

  return task;

  function task() {
    return g.src(src)
      .pipe(p.inline({
        base: dist,
        js: p.uglify(),
        css: p.minifyCss(),
      }))
      .on('error', p.util.log)
      .pipe(p.inlineImageHtml(imgSrc))
      .on('error', p.util.log)
      .pipe(p.minifyHtml())
      .on('error', p.util.log)
      .pipe(p.rename(newName))
      .pipe(g.dest(dist));
  }
}
