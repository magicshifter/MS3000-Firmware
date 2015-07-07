export function mocha(g, conf, p) {

  // Load globals first
  let src = [
    p.join(process.cwd(), conf.dirs.test, 'unit', 'globals.js'),
    p.join(process.cwd(), conf.dirs.src, 'js', 'gear'),
    p.join(process.cwd(), conf.dirs.test, 'unit', '**', '*.js'),
  ];

  return task;

  function task(done) {
    return g.src(src, {read: false})

        // Gulp-mocha needs filepaths, has to be the first pipe!
        .pipe(p.mocha({
          require: ['sinon', 'chai'],
        }));
  }
}
