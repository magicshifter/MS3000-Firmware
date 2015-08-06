import React from 'react';

import * as actions from './actions';
import Component from '../components/component.react';

export default class BrightnessControl extends Component {

  render() {
    const { brightness } = this.props;

    return (
      <input
        type='range'
        min='0'
        max='255'
        onChange={e => actions.changeBrightness(e)}
      />
    );
  }
}
