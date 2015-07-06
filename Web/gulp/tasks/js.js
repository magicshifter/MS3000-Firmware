export function js(g, conf, p) {
  let src = [
    p.join(process.cwd(), conf.dirs.dist, conf.dirs.js, 'tags.js'),
    p.join(process.cwd(), conf.dirs.src, conf.dirs.js, 'index.js'),
  ];
  let dist = p.join(conf.dirs.dist, conf.dirs.js);

  p.browserify().transform(p.babelify.configure({
    blacklist: ['react'],
  }));

  let b = p.browserify({
    entries: src,
    transform: [
      [p.babelify, { blacklist: ['react'] }],
    ],
  });

  return task;

  function task() {
    return b.bundle()
      .on('error', p.util.log)
      .pipe(p.source('index.js'))
      .on('error', p.util.log)
      .pipe(g.dest(dist));
  }
}
