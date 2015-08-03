import React from 'react';

import * as actions from './actions';
import Component from '../components/component.react';

import ColorPickerPreview from '../colorpicker/colorpickerpreview.react';

export default class Led extends Component {

  render() {
    const styles = {
      led: {
        float: 'left',
        height: '4.9vw',
        width: '4.9vw',
        display: 'block',
        border: '.5vw solid',
        borderColor: 'grey',
        margin: '.1vw',
      },
      active: {
        borderColor: 'lightgrey',
      },
    };

    const { value, active, ledId } = this.props;
    if (active) {
      styles.led.borderColor = styles.active.borderColor;
    }

    return (
      <div
        style={styles.led}
        onMouseUp={e => actions.toggleLed({ledId, active})}
        onDragEnter={e => actions.activateLed({e, ledId})}
      >
        <ColorPickerPreview
          value={value}
          active={active}
        />
      </div>
    );
  }
}
