import './colorpickerpreview.styl';
import Component from '../components/component.react';
import React from 'react';
import {msg} from '../intl/store';

import {toggleLed} from '../leds/actions';

export default class ColorPickerPreview extends Component {
  onClick(e) {
    e.preventDefault();
    console.log('toggle', this.props);
    toggleLed(this.props);
  }

  render() {
    const {value: backgroundColor, big, key} = this.props;
    let className = 'color-picker-preview';
    className += big ? ' big' : '';

    return (
      <span
        className={className}
        style={{
          backgroundColor,
        }}
        onClick={(e) => this.props.onClick(e)}
      ></span>
    );
  }
}
