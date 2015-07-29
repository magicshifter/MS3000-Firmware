import * as actions from './actions';
import Component from '../components/component.react';
import React from 'react';
import immutable from 'immutable';
import {msg} from '../intl/store';

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
