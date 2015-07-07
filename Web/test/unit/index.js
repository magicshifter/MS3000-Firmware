
describe('make sure index.js exports the expected functions', () => {
  it('should have utility functions defined', () => {
    let functions = [
      'arrayify',
      'each',
      'stopEvent',
      {
        lib: 'types',
        fns: [
          'is',
          'not',
          'isA',
          'isN',
          'isF',
          'isO',
          'isS',
          'isView',
        ],
      },
      {
        lib: 'request',
        fns: [
          'request',
          'get',
          'post',
          'postJSON',
          'postFile',
          'serializeURI',
        ],
      },
    ];

    functions.forEach(fn => {
      if (typeof fn === 'object') {
        fn.fns.forEach((f) => {
          expect(gear[fn.lib][f]).to.be.a('function');
        });
      } else {
        expect(gear[fn]).to.be.a('function');
      }
    });
  });
});
