import {Map} from 'immutable';

// Define how Leds initial state should be revived.
// http://facebook.github.io/immutable-js/docs/#/fromJS
export default function(value) {
  return Map(value);
}
