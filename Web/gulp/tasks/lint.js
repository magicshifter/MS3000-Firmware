export function lint(g, conf, p) {
  let src = [
    p.join(conf.dirs.gulp, '**', '*.js'),
    p.join(conf.dirs.src, conf.dirs.js, '**', '*.js'),
  ];
  let ignore = p.join(process.cwd(), conf.dirs.gulp, 'tasks.js');

  return () => {
    return g.src(src)
      .pipe(p.ignore.exclude(ignore))
      .pipe(p.jscs())

      // Suppress errors to pass them downwards
      .on('error', () => { null; })

      .pipe(p.jscsStylish());
  };
}
