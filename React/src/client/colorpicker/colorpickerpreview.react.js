import React from 'react';

import Component from '../components/component.react';
import {msg} from '../intl/store';

import * as actions from '../leds/actions';

import merge from 'magic-merge';

export default class ColorPickerPreview extends Component {

  onClick(e) {
    e.preventDefault();
    actions.toggleLed(this.props);
  }

  render() {
    const styles = {
      container: merge({
        display: 'inline-block',
        height: '100%',
        width: '100%',
        backgroundColor: '#000000',
      }, this.props.style),
    };

    const {value, size} = this.props;
    let containerStyle = styles.container;

    if (size) {
      containerStyle.width = size;
      containerStyle.height = size;
    }

    if (value) {
      containerStyle.backgroundColor = value;
    }

    return (
      <div style={containerStyle}></div>
    );
  }
}
