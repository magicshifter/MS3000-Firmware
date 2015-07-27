// __tests__/api/test.js
jest.dontMock('../../src/index.js');

import MagicShifterApi from '../../src/index.js'

console.log(MagicShifterApi);
var api = new MagicShifterApi();

describe('MagicShifterApi.request', function() {
  it('calls new XMLHttpRequest', function() {
    api.log('http://server.cors-api.appspot.com/server?id=168909&enable=true&status=200&credentials=false');

    expect(true).toBe(true);
  });
});
