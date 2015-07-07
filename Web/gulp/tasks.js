'use strict';

Object.defineProperty(exports, '__esModule', {
  value: true
});
exports.appcache = appcache;
exports.clean = clean;
exports.compress = compress;
exports.copy = copy;
exports.css = css;
exports.fontello = fontello;
exports.html = html;
exports.init = init;
exports.inline = inline;
exports.js = js;
exports.karma = karma;
exports.lint = lint;
exports.lintHtml = lintHtml;
exports.mocha = mocha;
exports.riot = riot;
exports.server = server;
exports.stylint = stylint;
exports.watch = watch;

function appcache(g, conf, p) {
  var src = p.join(conf.dirs.src, 'manifest.appcache');
  var dist = conf.dirs.dist;

  return task;

  function task() {
    return g.src(src).pipe(p.replace(/\|date\|/, new Date())).on('error', p.util.log).pipe(g.dest(dist));
  }
}

function clean(g, conf, p) {
  var dist = p.join(conf.dirs.dist, '*');

  return task;

  function task() {
    return g.src(dist, { read: false }).pipe(p.rimraf(dist)).on('error', p.util.log);
  }
}

function compress(g, conf, p) {
  var src = [p.join(conf.dirs.dist, 'index.html'), p.join(conf.dirs.dist, 'manifest.appcache'), p.join(conf.dirs.dist, 'favicon.gif')];
  var dist = conf.dirs.dist;

  return task;

  function task() {
    return g.src(src).pipe(p.gzip({
      gzipOptions: {
        level: 9,
        append: true
      }
    })).on('error', p.util.log).pipe(g.dest(dist));
  }
}

function copy(g, conf, p) {
  var src = [p.join(conf.dirs.src, 'favicon.gif'), p.join(conf.dirs.src, conf.dirs.img)];
  var dist = conf.dirs.dist;

  return task;

  function task() {
    return g.src(src).pipe(p.copy(dist, { prefix: 1 })).on('error', p.util.log);
  }
}

function css(g, conf, p) {
  var src = p.join(conf.dirs.src, conf.dirs.css);
  var cssSrc = p.join(src, 'main.styl');
  var cssImport = p.join('theme', 'colors.styl');
  var dist = p.join(conf.dirs.dist, conf.dirs.css);

  return task;
  function task() {
    return g.src(cssSrc).pipe(p.stylus({
      use: p.nib(),
      'import': ['nib', cssImport],
      'include css': true
    })).pipe(g.dest(dist));
  }
}

function fontello(g, conf, p) {
  var dist = p.join(conf.dirs.src, 'fontello');
  var config = p.join(process.cwd(), 'fontello.json');

  return task;
  function task() {
    return g.src(config).pipe(p.fontello(config)).on('error', p.util.log).pipe(g.dest(dist));
  }
}

function html(g, conf, p) {
  var src = p.join(conf.dirs.src, conf.dirs.html, conf.dirs.pages, '*.jade');
  var dist = conf.dirs.dist;

  return function task() {
    return g.src(src).pipe(p.jade({
      jade: p.compileJade,
      pretty: true
    })).on('error', p.util.log).pipe(g.dest(dist));
  };
}

function init(g, conf, p) {
  var src = [p.join(process.cwd(), conf.dirs.gulp, 'tasks', '**', '*.js')];
  var dist = p.join(conf.dirs.gulp);

  return task;

  function task() {
    return g.src(src).pipe(p.concat('tasks.js')).pipe(p.babel()).on('error', p.util.log).on('error', p.util.log).pipe(g.dest(dist));
  }
}

function inline(g, conf, p) {
  var src = p.join(conf.dirs.dist, 'home.html');
  var newName = 'index.html';
  var imgSrc = p.join(conf.dirs.dist, conf.dirs.img);
  var dist = conf.dirs.dist;

  return task;

  function task() {
    return g.src(src).pipe(p.inline({
      base: dist,
      js: p.uglify(),
      css: p.minifyCss()
    })).on('error', p.util.log).pipe(p.inlineImageHtml(imgSrc)).on('error', p.util.log).pipe(p.minifyHtml()).on('error', p.util.log).pipe(p.rename(newName)).pipe(g.dest(dist));
  }
}

function js(g, conf, p) {
  var src = [p.join(process.cwd(), conf.dirs.dist, conf.dirs.js, 'tags.js'), p.join(process.cwd(), conf.dirs.src, conf.dirs.js, 'index.js')];
  var dist = p.join(conf.dirs.dist, conf.dirs.js);

  p.browserify().transform(p.babelify.configure({
    blacklist: ['react']
  }));

  var b = p.browserify({
    entries: src,
    transform: [[p.babelify, { blacklist: ['react'] }]]
  });

  return task;

  function task() {
    return b.bundle().on('error', p.util.log).pipe(p.source('index.js')).on('error', p.util.log).pipe(g.dest(dist));
  }
}

function karma(g, conf, p) {
  var config = p.join(process.cwd(), conf.dirs.test, 'karma.conf.js');

  return task;

  function task(done) {
    p.karma.start({
      configFile: config,
      singleRun: true
    }, done);
  }
}

function lint(g, conf, p) {
  var src = [p.join(conf.dirs.gulp, '**', '*.js'), p.join(conf.dirs.src, conf.dirs.js, '**', '*.js')];
  var ignore = p.join(process.cwd(), conf.dirs.gulp, 'tasks.js');

  return function () {
    return g.src(src).pipe(p.ignore.exclude(ignore)).pipe(p.jscs())

    // Suppress errors to pass them downwards
    .on('error', function () {
      null;
    }).pipe(p.jscsStylish());
  };
}

function lintHtml(g, conf, p) {
  var src = p.join(conf.dirs.dist, '*.html');

  return task;

  function task() {
    return g.src(src).pipe(p.htmlhint());
  }
}

function mocha(g, conf, p) {

  // Load globals first
  var src = [p.join(process.cwd(), conf.dirs.test, 'unit', 'globals.js'), p.join(process.cwd(), conf.dirs.src, 'js', 'gear'), p.join(process.cwd(), conf.dirs.test, 'unit', '**', '*.js')];

  return task;

  function task(done) {
    return g.src(src, { read: false })

    // Gulp-mocha needs filepaths, has to be the first pipe!
    .pipe(p.mocha({
      require: ['sinon', 'chai']
    }));
  }
}

function riot(g, conf, p) {
  var src = p.join(conf.dirs.src, conf.dirs.components, '**', '*.jade');
  var dist = p.join(conf.dirs.dist, conf.dirs.js);

  return task;

  function task() {
    return g.src(src).pipe(p.riot({
      template: 'jade',
      type: 'es6',
      ext: 'jade'
    })).on('error', p.util.log).pipe(p.concat('tags.js')).on('error', p.util.log)
    /* Add the riot require as the first statement of our new js file,
     * using var because we are not in es6 land anymore
     */
    .pipe(p.injectString.prepend('import riot from \'riot\';\n')).on('error', p.util.log).pipe(g.dest(dist));
  }
}

function server(g, conf, p) {
  var dist = p.join(process.cwd(), conf.dirs.dist);

  return task;

  function task(done) {
    var mount = p.st({
      path: dist,
      url: '/',
      cache: false
    });

    p.mockyShifter(mount).listen(conf.port, done);
  }
}

function stylint(g, conf, p) {
  var src = p.join(conf.dirs.src, conf.dirs.css, '*.styl');

  return task;
  function task() {
    return g.src(src).pipe(p.stylint());
  }
}

function watch(g, conf, p) {
  var srcDir = conf.dirs.src;
  var src = p.join(srcDir, '**', '*');
  var gulpSrc = [p.join(conf.dirs.gulp, '**', '*.js'), 'config.js', 'gulpfile.js', conf.dirs.test];
  var mockyShifterDir = p.join('mockyshifter', '**', '*');

  return task;
  function task() {
    p.watch(src, function () {
      g.start('compress');
    });

    p.watch(gulpSrc, function () {
      g.start('compress');
    });

    p.watch(mockyShifterDir, function () {
      g.start('compress');
    });
  }
}