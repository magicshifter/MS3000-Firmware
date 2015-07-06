export function init(g, conf, p) {
  let src = [
    p.join(process.cwd(), conf.dirs.gulp, 'tasks', '**', '*.js'),
  ];
  let dist = p.join(conf.dirs.gulp);

  return task;

  function task() {
    return g.src(src)
      .pipe(p.concat('tasks.js'))
      .pipe(p.babel())
      .on('error', p.util.log)
      .on('error', p.util.log)
      .pipe(g.dest(dist));
  }
}
