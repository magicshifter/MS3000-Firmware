import React from 'react';

import * as actions from './actions';
import Component from '../components/component.react';

import {msg} from '../intl/store';

export default class BrightnessControl extends Component {

  render() {
    const { brightness} = this.props;

    return (
      <div>
        <h4>{msg('pages.leds.brightness')}</h4>
        <input
          type='range'
          min='1'
          max='31'
          onChange={e => actions.changeBrightness(e)}
        />
        <label>{brightness}</label>
      </div>
    );
  }
}
