import React from 'react';

import * as actions from './actions';
import Component from '../components/component.react';
import LedPreview from '../components/ledpreview.react';

import merge from 'magic-merge';

export default class Led extends Component {

  render() {
    const { value, active, ledId, style } = this.props;

    const styles = {
      led: merge({
        float: 'left',
        height: '4.9vw',
        width: '4.9vw',
        display: 'block',
        border: '.5vw solid',
        borderColor: active ? 'lightgrey' : 'grey',
        margin: '.1vw',
        maxHeight: '50px',
        maxWidth: '50px',
      }, style),
    };

    return (
      <div
        style={styles.led}
        onMouseUp={e => actions.toggleLed({ledId, active})}
      >
        <LedPreview
          value={value}
        />
      </div>
    );
  }
}
