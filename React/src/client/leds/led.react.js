import './led.styl';
import * as actions from './actions';
import Component from '../components/component.react';
import React from 'react';
import immutable from 'immutable';
import {msg} from '../intl/store';

import ColorPickerPreview from '../colorpicker/colorpickerpreview.react';
import ColorPicker from '../colorpicker/colorpicker.react';

export default class Led extends Component {

  render() {
    const { value, active } = this.props;
    const className = active ? 'led active' : 'led';

    return (
      <div className={className}>
        <ColorPickerPreview
          onClick={(e) => actions.toggleLed(this.props)}
          big={true}
          {...this.props}
        />
      </div>
    );
  }

}
