'use strict';
import gulp from 'gulp';
import loadPlugins from 'load-plugins';
import {readdirSync, existsSync} from 'fs-extra';
import {join} from 'path';
import source from 'vinyl-source-stream';
import {server as karma} from 'karma';
import nib from 'nib';
import jade from 'jade';
import babelify from 'babelify';
import browserify from 'browserify';
import st from 'st';
import mockyShifter from '../mockyshifter';
import {argv} from 'yargs';

import config from '../config';

let plugins = loadPlugins('gulp-*', {camelize: true});
plugins.source = source;
plugins.karma = karma;
plugins.join = join;
plugins.nib = nib;
plugins.compileJade = jade;
plugins.browserify = browserify;
plugins.babelify = babelify;
plugins.st = st;
plugins.mockyShifter = mockyShifter;

let taskList = [];
let tasks = readdirSync(join(__dirname, 'tasks'));
tasks.forEach((task) => {
  taskList[task] = tasks[task];
});

tasks = {};
Object.keys(taskList).forEach((key) => {
  key = key.replace('.js', '');
  let taskSrc = join(__dirname, 'tasks', key);
  let task = require(taskSrc);
  if (!task || typeof task[key] !== 'function') {
    return console.log('Task is not a function:', key);
  }

  tasks[key] = task[key](gulp, config, plugins);
});

export default tasks;
