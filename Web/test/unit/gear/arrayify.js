let arrayify = gear.arrayify;

describe('#arrayify(object, function)', () => {
  it('should not error if called without arguments', () => {
    let err = false;
    try { arrayify(); }
    catch (e) { err = e; }
    expect(err).to.be.false;
  });
  it('should correctly turn an object into an array', () => {
    let obj = {
      key1: 'value1',
      key2: {key21: 'value21'},
    };
    let arr = arrayify(obj);
    expect(arr).to.be.an('array');
  });

  it('should correctly set the key:values', () => {
    let obj = {
      key1: 'value1',
      key2: {key21: 'value21'},
    };
    let testArr = [
      {key: 'key1', value: 'value1'},
      {
        key: 'key2',
        value: [
          {key: 'key21', value: 'value21'},
        ],
      },
    ];
    let arr = arrayify(obj);
    expect(arr[0].key).to.equal(testArr[0].key);
    expect(arr[0].value).to.equal(testArr[0].value);
    expect(arr[1].key).to.equal(testArr[1].key);
    expect(arr[1].value[0].key).to.equal(testArr[1].value[0].key);
    expect(arr[1].value[0].value).to.equal(testArr[1].value[0].value);
  });
});
