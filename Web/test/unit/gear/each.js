let each = gear.each;

let arr = ['value1', 'value2'];
let arrTestArr = [ '0', '1' ];
let obj = {
  key1: 'value1',
  key2: 'value2',
};
let objTestArr = ['key1', 'key2'];

describe('#each(object, function)', () => {
  it('should not error if called without arguments', () => {
    var fnError = false;
    try { each(obj); }
    catch (e) { fnError = true; }
    expect(fnError).to.be.false;

    var argError = false;
    try { each(); }
    catch (e) { argError = true; }
    expect(argError).to.be.false;
  });

  it('should loop over each item in an array', () => {
    var cnt = 0;

    each(arr, (val, key) => {
      expect(key).to.equal(arrTestArr[cnt]);
      cnt++;
    });
    expect(cnt).to.equal(2);
  });

  it('should loop over each item in an object', () => {
    var cnt = 0;

    each(obj, (val, key) => {
      expect(key).to.equal(objTestArr[cnt]);
      cnt++;
    });
    expect(cnt).to.equal(2);
  });
});
