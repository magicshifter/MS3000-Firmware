import compression from 'compression';
import config from '../config';
import headers from '../lib/headers';
import express from 'express';
import favicon from 'serve-favicon';
import render from './render';
import userState from './userstate';

const app = express();

// Add Este.js headers for React related routes only.
if (!config.isProduction)
  app.use(headers());

app.use(compression());
app.use(favicon('assets/favicon.ico'));

app.use('/build', express.static('build'));
app.use('/assets', express.static('assets'));

// Load state extras for current user.
app.use(userState());

app.get('*', (req, res, next) => {
  render(req, res, req.userState).catch(next);
});

app.on('mount', () => {
  console.log('App is available at %s', app.mountpath);
});

export default app;
