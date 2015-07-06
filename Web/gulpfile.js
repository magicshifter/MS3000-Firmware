'use strict';

var gulp = require('gulp');
var help = require('gulp-task-listing');

require('./node_modules/babelify/node_modules/babel-core/register');
var tasks = require('./gulp/');

gulp.task('build', [
  'build:copy',
  'build:appcache',
  'build:html',
  'build:css',
  'build:js',
]);

gulp.task('default', [
  'server',
  'compress',
  'watch',
]);

gulp.task('help', help);

gulp.task('lint', ['lint:css', 'lint:js']);

gulp.task('init', ['fontello'], tasks.init);

gulp.task('build:html', tasks.html);
gulp.task('build:riot', tasks.riot);

gulp.task('lint:css', tasks.stylint);
gulp.task('build:css', ['lint:css'], tasks.css);

gulp.task('fontello', tasks.fontello);

gulp.task('build:riot', tasks.riot);

gulp.task('lint:js', tasks.lint);
gulp.task('build:js', ['lint:js', 'build:riot'], tasks.js);

gulp.task('build:appcache', tasks.appcache);

gulp.task('build:copy', tasks.copy);

gulp.task('inline', ['build:js', 'build:css', 'build:html'], tasks.inline);

gulp.task('compress', ['build', 'inline', 'test:mocha'], tasks.compress);

gulp.task('build:clean', tasks.clean);
gulp.task('clean', ['build:clean', 'init']);

gulp.task('server', tasks.server);

gulp.task('test', ['test:mocha', 'test:karma']);
gulp.task('test:mocha', tasks.mocha);
gulp.task('test:karma', tasks.karma);

gulp.task('watch', tasks.watch);
