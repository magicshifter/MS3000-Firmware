import riot from 'riot';
import gear from './gear';

class Log {
  constructor(hq) {
    this.maxMessages = 10;
    this._hq = hq;
    this._messages = [];

    riot.observable(this);
    this._hq.enlist(this);

    this.currentMessage = false;
  }

  put(msg = '', type='info') {
    if (gear.types.isO(msg) && msg.text) {
      type = msg.type || type;
      msg = msg.text;
    }

    let lastMsg = { _id: 0 };
    if (this._messages.length) {
      let testLastMsg = this._messages[this._messages.length - 1];
      if (testLastMsg && testLastMsg._id) {
        lastMsg = testLastMsg;
      }
    }

    let msg = {type, text: msg, _id: lastMsg._id + 1};
    this._messages.push(msg);
    if (this._messages.length > this.maxMessages) {
      this._messages.pop();
    }

    this._hq.trigger('log:message:put', msg);
  }

  del(msgId) {
    msgId = parseInt(msgId);
    if (msgId !== msgId) { return false; }

    this._messages = this._messages.filter((msg) => msg._id !== msgId);
    this._hq.trigger('log:message:del');
  }

  get messages() {
    return this._messages;
  }

  set messages(msg) {
    console.warn('Setting Log.messages directly is not supported');
  }
}
export default Log;
