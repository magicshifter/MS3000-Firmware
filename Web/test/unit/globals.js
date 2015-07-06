import chai from 'chai';
import sinon from 'sinon';
import gear from '../../src/js/gear';

global.expect = chai.expect;
global.sinon = sinon;
global.spy = sinon.spy;
global.gear = gear;

sinon.assert.expose(chai.assert, { prefix: '' });
