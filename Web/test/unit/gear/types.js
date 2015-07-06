describe('types', () => {
  describe('#isA(val)', () => {
    let isA = gear.types.isA;
    it('should return true for arrays', () => {
      expect(isA([])).to.be.true;
    });
    it('should return false for other types, including objects', () => {
      let vals = [0, 1, '', 'test', {}, function() {}];
      vals.forEach((val) => {
        expect(isA(val)).to.be.false;
      });
    });
  });

  describe('#isO(val)', () => {
    let isO = gear.types.isO;
    it('should return true for objects and arrays', () => {
      expect(isO({})).to.be.true;
      expect(isO([])).to.be.true;
    });
    it('should return false for other types', () => {
      let vals = [0, 1, 'test', '', function() {}];
      vals.forEach((val) => {
        expect(isO(val)).to.be.false;
      });
    });
  });

  describe('#isN(val)', () => {
    let isN = gear.types.isN;
    it('should return true for 0, int and float', () => {
      expect(isN(0)).to.be.true;
      expect(isN(1)).to.be.true;
      expect(isN(1.1)).to.be.true;
    });
    it('should return false for other types', () => {
      let vals = ['test', '', {}, [], function() {}];
      vals.forEach((val) => {
        expect(isN(val)).to.be.false;
      });
    });
  });

  describe('#isS(val)', () => {
    let isS = gear.types.isS;
    it('should return true for "", "1" and "strings"', () => {
      expect(isS('')).to.be.true;
      expect(isS('1')).to.be.true;
      expect(isS('string')).to.be.true;
    });
    it('should return false for other types', () => {
      let vals = [0, 1, [], {}, function() {}];
      vals.forEach((val) => {
        expect(isS(val)).to.be.false;
      });
    });
  });

  describe('#isF(val)', () => {
    let isF = gear.types.isF;
    it('should return true for functions', () => {
      expect(isF(function() {})).to.be.true;
    });
    it('should return false for other types', () => {
      let vals = [0, 1, [], {}, '', 'test'];
      vals.forEach((val) => {
        expect(isF(val)).to.be.false;
      });
    });
  });

  describe('#isView(val)', () => {
    let isView = gear.types.isView;
    it('returns true if value has a trigger function', () => {
      let mock = { trigger: function() {} };
      expect(isView(mock)).to.be.true;
    });
    it('returns false for other types', () => {
      let vals = ['', 'test', 0, 1, [], {}, function() {}];
      vals.forEach((val) => {
        expect(isView(val)).to.be.false;
      });
    });
  });

  describe('#is(val)', () => {
    let is = gear.types.is;
    it('returns true if v is not undefined', () => {
      let mock = {test: false};
      expect(is(mock.test)).to.be.true;
    });
    it('returns false if v is undefined', () => {
      let mock = {};
      expect(is(mock.test)).to.be.false;
    });
  });

  describe('#is(val, type)', () => {
    let is = gear.types.is;
    it('returns true for correct typechecks', () => {
      let vals = [
        { type: 'object', val: {} },
        { type: 'number', val: 0 },
        { type: 'string', val: 'test' },
        { type: 'function', val: function() {} },
      ];
      vals.forEach((val) => {
        expect(is(val.val, val.type)).to.be.true;
      });
    });
    it('returns false for wrong typechecks', () => {
      let vals = [
        { type: 'string', val: {} },
        { type: 'object', val: 0 },
        { type: 'function', val: 'test' },
        { type: 'number', val: function() {} },
      ];
      vals.forEach((val) => {
        expect(is(val.val, val.type)).to.be.false;
      });
    });
  });

  describe('#not(val)', () => {
    let not = gear.types.not;
    it('returns true if v is undefined', () => {
      let mock = {};
      expect(not(mock.test)).to.be.true;
    });
    it('returns false if v is undefined', () => {
      let mock = { test: false };
      expect(not(mock.test)).to.be.false;
    });
  });

  describe('#not(val, type)', () => {
    let not = gear.types.not;
    it('returns false for correct typechecks', () => {
      let vals = [
        { type: 'object', val: {} },
        { type: 'number', val: 0 },
        { type: 'string', val: 'test' },
        { type: 'function', val: function() {} },
      ];
      vals.forEach((val) => {
        expect(not(val.val, val.type)).to.be.false;
      });
    });
    it('returns true for wrong typechecks', () => {
      let vals = [
        { type: 'string', val: {} },
        { type: 'object', val: 0 },
        { type: 'function', val: 'test' },
        { type: 'number', val: function() {} },
      ];
      vals.forEach((val) => {
        expect(not(val.val, val.type)).to.be.true;
      });
    });
  });
});
