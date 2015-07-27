import P from 'es6-promise';
P.polyfill();

/**
 * The MagicShifter default configuration.
 * To override:
 * const config = {hostname: 'host.local', port: 8080};
 * const api = new MagicShifterApi(config);
 */
const defaultConfig = {
  protocol: 'http',
  hostname: 'magicshifter.local',
  port: 80,
};

function merge(obj1, obj2) {
  Object.keys(obj2).forEach((key) => {
    if (typeof obj2[key] === 'object') {
      mergeObjects(obj1[key], obj2[key]);
    } else {
      obj1[key] = obj2[key];
    }
  });

  return obj1;
}

/**
 * The Root MagicShifter Api class.
 */
export default class MagicShifterApi {

  /**
   * MagicShifterApi constructor function.
   * @param {object} [config='{hostname: 'magicshifter.local', port: 80}' - configuration object.
   */
  constructor(config = {}) {
    this.config = merge(config, defaultConfig);
  }

  connect(cb) {
    this.request('/info')
        .then(cb, cb);
  }

  discover(cb) {
    const defaultHost = 'magicshifter.local';
    const lookupTable = [
      this.ip, //accesspoint mode
      defaultHost, //default hostname
      this.host !== this.ip && this.host !== defaultHost && this.host,
    ];
    console.log(lookupTable);
  }

  /*
   * Requests url and logs the result
   * @param {string} [url=''] - absolute url to request (example: /leds)
  */
  log(url = '') {
    this
      .request(url)
      .then(
        (data, xhr) => {
          console.log(JSON.stringify(data));
        },
        (data, xhr) => {
          console.error(data);
        }
      );
  }

  /**
   * request
   * @param {string} [url=''] - absolute url to request (example: /leds)
   * @param {string|object} [params=''] - if object will be encodeURIComponent
                                          encoded and will be appended to the
                                          url
   * @param {object} [postData=false] - postData to send via xhr.send after
                                        opening the xhr
   * @returns {XMLHttpRequest} - the xhr object used in the request
   */
  request(url, params = '', postData = '') {
    const xhr = new XMLHttpRequest();

    url = this.encodeURI(url, params);

    if (!url || url.indexOf('http') !== 0) {
      console.error('Url is not a valid absolute url.', url);
      return false;
    }

    return new Promise((resolve, reject) => {
      // Do the usual XHR stuff
      const req = new XMLHttpRequest();
      req.open('GET', url);
      req.timeout = 4000;

      req.onload = () => {
        // This is called even on 404 etc
        // so check the status
        if (req.status === 200) {
          // Resolve the promise with the response text
          resolve(null, req.response);
        } else {
          // Otherwise reject with the status text
          // which will hopefully be a meaningful error
          reject(Error(req.statusText), req.response);
        }
      };

      // Handle network errors
      req.onerror = () => {
        reject(Error('Network Error'));
      };

      // Handle request timeout
      req.ontimeout = () => {
        reject(Error('Request Timeout'));
      };


      // Make the request
      req.send(postData);
    });
  }

  encodeURI(url, params = '') {
    const {port, hostname, protocol} = this.config;

    if (!url) { return false; }

    if (url.indexOf('/') !== 0) {
      return false;
    }

    if (port !== 80) {
      url = `:${port}${url}`;
    }

    url = `${protocol}://${hostname}${url}`;

    if (params) {
      if (typeof params === 'string') {
        params = encodeURIComponent(params);
      } else if (typeof params === 'object') {
        let qs = '?';

        Object.keys(params).forEach((key) => {
          qs += `${key}=${params[key]}`;
        });
        params = encodeURIComponent(qs);
      }

      url = url + params;
    }

    return url;
  }
}

if (window) { window.MagicShifterApi = MagicShifterApi; }
