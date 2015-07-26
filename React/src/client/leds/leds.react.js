import './leds.styl';
import * as actions from './actions';
import Component from '../components/component.react';
import React from 'react';
import immutable from 'immutable';
import {msg} from '../intl/store';

import Led from './led.react';
import ColorPicker from '../colorpicker/colorpicker.react';
import ColorList from '../colorpicker/colorlist.react';

export default class LedsForm extends Component {

  render() {
    const { leds } = this.props;
    const list = leds.get('list');
    let ledsHtml = [];
    list.map((val, key) => {
      ledsHtml.push(
        <Led
          key={key}
          id={key}
          value={val.get('value')}
          active={val.get('active')}
          {...this.props}
        />
      );
    });

    return (
      <div className="leds">
        <div className='leds-list'>
          {ledsHtml}
        </div>
        <div className='controls'>
          <ColorPicker {...this.props} />
          <ColorList {...this.props} />
        </div>
      </div>
    );
  }
}
