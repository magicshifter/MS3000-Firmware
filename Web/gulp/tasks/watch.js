export function watch(g, conf, p) {
  let srcDir = conf.dirs.src;
  let src = p.join(srcDir, '**', '*');
  let gulpSrc = [
    p.join(conf.dirs.gulp, '**', '*.js'),
    'config.js',
    'gulpfile.js',
    conf.dirs.test,
  ];
  let mockyShifterDir = p.join('mockyshifter', '**', '*');

  return task;
  function task() {
    p.watch(src, () => {
      g.start('compress');
    });

    p.watch(gulpSrc, () => {
      g.start('compress');
    });

    p.watch(mockyShifterDir, () => {
      g.start('compress');
    });
  }
}
