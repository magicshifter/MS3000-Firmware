let stopEvent = gear.stopEvent;

describe('#stopEvent(evt)', () => {
  it('does not error if called without arguments', () => {
    let err = false;
    try { stopEvent(); }
    catch (e) { err = e;}
    expect(err).to.be.false;
  });

  it('stops an event from propagating', () => {
    var evt = {
      stopPropagation: sinon.spy(),
      preventDefault: sinon.spy(),
    };

    let returnFalse = stopEvent(evt);

    expect(evt.stopPropagation.calledOnce).to.be.true;
    expect(evt.preventDefault.calledOnce).to.be.true;
    expect(returnFalse).to.be.false;
  });

  it('stops events even in weird browsers', () => {
    var evt = {
      cancelBubble: false,
      returnValue: true,
    };

    let returnFalse = stopEvent(evt);

    expect(evt.cancelBubble).to.be.true;
    expect(evt.returnValue).to.be.false;
    expect(returnFalse).to.be.false;
  });
});
