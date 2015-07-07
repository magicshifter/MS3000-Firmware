let mixins = gear.mixins;

describe('riot mixins', () => {

  describe('#isActive()', () => {
    it('returns this.active', () => {
      let self = {
        active: true,
      };
      let returnVal = mixins.isActive.call(self);
      expect(returnVal).to.be.true;
    });
  });

  describe('#show()', () => {
    it('sets this.active = true and calls this.update', () => {
      let self = {
        active: false,
        update: spy(),
      };
      mixins.show.call(self);
      expect(self.active).to.be.true;
      expect(self.update.called).to.be.true;
    });
  });

  describe('#hide()', () => {
    it('sets this.active = false and calls this.update', () => {
      let self = {
        active: true,
        update: spy(),
      };
      mixins.hide.call(self);
      expect(self.active).to.be.false;
      expect(self.update.called).to.be.true;
    });
  });
});
