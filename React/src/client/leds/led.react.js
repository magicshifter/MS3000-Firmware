import React from 'react';

import * as actions from './actions';
import Component from '../components/component.react';


import ColorPickerPreview from '../colorpicker/colorpickerpreview.react';
import ColorPicker from '../colorpicker/colorpicker.react';

export default class Led extends Component {

  render() {
    const styles = {
      led: {
        float: 'left',
      },
    };

    const { value, active } = this.props;
    const className = active ? 'active' : '';

    return (
      <div className={className} style={styles.led}>
        <ColorPickerPreview
          onClick={(e) => actions.toggleLed(this.props)}
          big={true}
          {...this.props}
        />
      </div>
    );
  }

}
