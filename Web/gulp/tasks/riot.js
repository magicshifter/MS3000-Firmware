export function riot(g, conf, p) {
  let src = p.join(conf.dirs.src, conf.dirs.components, '**', '*.jade');
  let dist = p.join(conf.dirs.dist, conf.dirs.js);

  return task;

  function task() {
    return g.src(src)
      .pipe(p.riot({
        template: 'jade',
        type: 'es6',
        ext: 'jade',
      }))
      .on('error', p.util.log)
      .pipe(p.concat('tags.js'))
      .on('error', p.util.log)
      /* Add the riot require as the first statement of our new js file,
       * using var because we are not in es6 land anymore
       */
      .pipe(p.injectString.prepend('import riot from \'riot\';\n'))
      .on('error', p.util.log)
      .pipe(g.dest(dist));
  }
}
