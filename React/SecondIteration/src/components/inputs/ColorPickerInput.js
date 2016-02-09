import React, {Component, PropTypes} from 'react';
import rgba from 'rgba-convert';
import {Saturation, Hue} from 'react-color/lib/components/common';

import {colorType, hexStringType} from 'utils/propTypes';

import classes from './ColorPickerInput.scss';

export default class ColorPickerInput extends Component {

  static propTypes = {
    color: PropTypes.oneOfType([colorType, hexStringType]).isRequired,
  };

  render() {
    const {color} = this.props;

    return (
      <div className={classes['container']}>
        <div className={classes['saturation']}>
          <Saturation
            {...this.props}
            color={rgba.hex(color)}
            direction='vertical'
          />
        </div>
        <div className={classes['hue']}>
          <Hue
            {...this.props}
            color={rgba.hex(color)}
            direction='vertical'
          />
        </div>
      </div>
    );
  }
}
