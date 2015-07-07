export function karma(g, conf, p) {
  let config = p.join(process.cwd(), conf.dirs.test, 'karma.conf.js');

  return task;

  function task(done) {
    p.karma.start({
      configFile: config,
      singleRun: true,
    }, done);
  }
}
