export function css(g, conf, p) {
  let src = p.join(conf.dirs.src, conf.dirs.css);
  let cssSrc = p.join(src, 'main.styl');
  let cssImport = p.join('theme', 'colors.styl');
  let dist = p.join(conf.dirs.dist, conf.dirs.css);

  return task;
  function task() {
    return g.src(cssSrc)
      .pipe(p.stylus({
        use: p.nib(),
        import: ['nib', cssImport],
        'include css': true,
      }))
      .pipe(g.dest(dist));
  }
}
