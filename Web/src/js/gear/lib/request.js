import {isF} from './types';
import {noop} from './noop';

/* Function request
 * a convenience wrapper around XMLHttpRequest.
 *
 * @param {String} url - the url to request
 * @param {String=} method=GET - the http method to use
 * @param {Object} args - arguments to use,
  args.headers sets requestheaders
 * @param {Function} cb - callback to call on timeout or return of the request
 *
 * @returns {String} res - returns a string with the responseText
 * @returns {Error} err - Error if something went wrong
 */
export function request(url, method = 'GET', args = noop, cb = false) {
  let xhr = new XMLHttpRequest();
  xhr.timeout = args.timeout || 4000;

  if (!isF(cb) && isF(args)) {
    cb = args;
    args = null;
  }

  if (args && args.headers && isF(args.headers.forEach)) {
    args.headers.forEach((key) => {
      xhr.setRequestHeader(key, args.headers[key]);
    });
  }

  xhr.onreadystatechange = () => {
    if (xhr.readyState === 4) {
      let err;
      let status = xhr.status;
      if (status >= 400 && status <= 599) {
        err = `HTTP ${status} Error`;
      }

      if (status === 200) {
        if (!xhr.responseText) {
          err = 'Empty response.';
        } else if (xhr.responseText.toLowerCase() === 'ok') {
          xhr.ok = true;
        }
      }

      cb(err, xhr);
    }
  };

  xhr.ontimeout = () => { cb('Request timeout.', xhr); };

  xhr.open(method.toUpperCase(), url, true);
  return xhr;
}

/* Function get
 * a convenience wrapper around XMLHttpRequest.
 *
 * @param {String} url - the url to request
 * @param {Function} cb - callback to call on timeout or return of the request
 *
 * @returns {Error} err - Error if something went wrong
 * @returns {String} res - returns a string with the responseText
 */
export function get(url, cb) {
  let xhr = request(url, 'GET', cb);
  xhr.send(null);
}

/* Function post
 * a convenience wrapper around XMLHttpRequest.
 *
 * @param {String} url - the url to request
 * @param {Object} params - object with key value pairs
 * @param {Function} cb - callback to call on timeout or return of the request
 *
 * @returns {Error} err - Error if something went wrong
 * @returns {String} res - returns a string with the responseText
 */
export function post(url, params, cb) {
  let xhr = request(url, 'POST', cb);
  if (typeof params === 'object') {
    params = serializeURI(params);
  }

  xhr.send(params);
}

/* Function getJSON
 * json stringify an object and return it
 *
 * @param {String} url - the url to request
 * @param {Function} cb - callback to call on timeout or return of the request
 *
 * @returns {Error} err - Error if something went wrong
 * @returns {String} res - returns a string with the responseText
 */
export function getJSON(url, cb = noop) {
  let xhr = request(url, 'GET', (err, xhr) => {
    if (xhr.responseText) {
      try {
        xhr.json = JSON.parse(xhr.responseText);
      }
      catch (e) {
        xhr.json = false;
      }
    }

    cb(err, xhr);
  });

  xhr.send(null);
}

/* Function postJSON
 * json stringify an object and post it
 *
 * @param {String} url - the url to request
 * @param {Object} params - object with key value pairs
 * @param {Function} cb - callback to call on timeout or return of the request
 *
 * @returns {Error} err - Error if something went wrong
 * @returns {String} res - returns a string with the responseText
 */
export function postJSON(url, params, cb = noop) {
  let newUrl = url + '?j=';
  if (typeof params !== 'string') {
    newUrl += JSON.stringify(params);
  }

  let xhr = request(newUrl, 'POST', cb);
  xhr.send(null);
}

export function postFile(url, file, cb) {
  if (Object.prototype.toString.call(file) !== '[object Array]') {
    file = [file];
  }

  var formData = new FormData();

  // JavaScript file-like object
  var blob = new Blob(file, { type: 'text/plain'});

  formData.append('file', blob);

  post(url, formData, cb);
  request.send(formData);
}

/* Function serializeURI
 * Given an Object this function returns a url encoded string.
 * {name: 'test', test: 'name'} becomes name=test&test=name
 *
 * @param {Object} obj - the Object to serialize
 *
 * @returns {Boolean, String, null} array - The serialized string.
 */
export function serializeURI(obj) {
  if (typeof obj === 'string') { return obj; }

  if (typeof obj !== 'object') { return null; }

  let str = [];
  let e = encodeURIComponent;
  Object.keys(obj).forEach((key) => {
    str.push(`${e(key)}=${e(obj[key])}`);
  });

  return str.join('&');
}

export function jsonifyResponse(err, xhr) {
  let res;
  if (!err && !xhr.responseText) {
    err = 'Empty response from MagicShifter.';
  }

  if (!err && xhr.responseText) {
    try {
      res = JSON.parse(xhr.responseText);
    }
    catch (e) {
      err = 'Invalid response from MagicShifter.';
    }
  }

  return {err, res};
}
