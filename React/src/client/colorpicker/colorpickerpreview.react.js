import React from 'react';

import Component from '../components/component.react';
import {msg} from '../intl/store';

import merge from 'magic-merge';

import * as actions from '../leds/actions';

export default class ColorPickerPreview extends Component {
  onClick(e) {
    e.preventDefault();
    actions.toggleLed(this.props);
  }

  render() {
    const styles = {
      container: {
        display: 'inline-block',
      },
      normal: {
        width: '20px',
        height: '20px',
        margin: '1px',
        border: '3px solid grey',
      },
      big: {
        width: '40px',
        height: '40px',
        margin: '2px',
        border: '5px solid grey',
      },
      active: {
        borderColor: 'lightgrey',
      },
    };

    const {value: backgroundColor, big, key} = this.props;
    let containerStyle = styles.container;
    if (big) {
      containerStyle = merge(containerStyle, styles.big);
    } else {
      containerStyle = merge(containerStyle, styles.normal);
    }

    containerStyle.backgroundColor = backgroundColor || '#000000';

    return (
      <span
        style={containerStyle}
        onClick={(e) => this.props.onClick(e)}
      ></span>
    );
  }
}
