(function e(t,n,r){function s(o,u){if(!n[o]){if(!t[o]){var a=typeof require=="function"&&require;if(!u&&a)return a(o,!0);if(i)return i(o,!0);var f=new Error("Cannot find module '"+o+"'");throw f.code="MODULE_NOT_FOUND",f}var l=n[o]={exports:{}};t[o][0].call(l.exports,function(e){var n=t[o][1][e];return s(n?n:e)},l,l.exports,e,t,n,r)}return n[o].exports}var i=typeof require=="function"&&require;for(var o=0;o<r.length;o++)s(r[o]);return s})({1:[function(require,module,exports){
'use strict';

exports.__esModule = true;

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError('Cannot call a class as a function'); } }

var defaultConfig = {
  hostname: 'magicshifter.local',
  port: 80
};

var MagicShifterApi = (function () {
  function MagicShifterApi() {
    var config = arguments.length <= 0 || arguments[0] === undefined ? defaultConfig : arguments[0];

    _classCallCheck(this, MagicShifterApi);

    console.log(config);
  }

  MagicShifterApi.prototype.log = function log() {
    var url = arguments.length <= 0 || arguments[0] === undefined ? '' : arguments[0];

    this.get(url).then(function (data, xhr) {
      console.log(JSON.stringify(data));
    }, function (data, xhr) {
      console.error(data);
    });
  };

  MagicShifterApi.prototype.get = function get(a) {
    // a   url (naming it a, because it will be reused to store callbacks)
    // xhr placeholder to avoid using var, not to be used
    var xhr = new XMLHttpRequest();

    // Open url
    xhr.open('GET', a);

    // Reuse a to store callbacks
    a = [];

    // onSuccess handler
    // onError   handler
    xhr.onreadystatechange = xhr.then = function (onSuccess, onError) {

      // Test if onSuccess is a function
      if (typeof onSuccess === 'function') {
        a = [, onSuccess, onError];
      }

      // Test if request is complete
      if (xhr.readyState == 4) {

        // index will be:
        // 0 if undefined
        // 1 if status is between 200 and 399
        // 2 if status is over
        var cb = a[0 | xhr.status / 200];

        // Safari doesn't support xhr.responseType = 'json'
        // so the response is parsed
        if (cb) {
          try {
            cb(JSON.parse(xhr.responseText), xhr);
          } catch (e) {
            cb(null, xhr);
          }
        }
      }
    };

    // Send
    xhr.send();

    // Return request
    return xhr;
  };

  return MagicShifterApi;
})();

exports['default'] = MagicShifterApi;
module.exports = exports['default'];

},{}],2:[function(require,module,exports){
'use strict';

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { 'default': obj }; }

var _indexJs = require('../index.js');

var _indexJs2 = _interopRequireDefault(_indexJs);

var api = new _indexJs2['default']();

api.log('http://server.cors-api.appspot.com/server?id=168909&enable=true&status=200&credentials=false');

if (window) {
  window.MagicShifterApi = api;
}

},{"../index.js":1}]},{},[2]);
