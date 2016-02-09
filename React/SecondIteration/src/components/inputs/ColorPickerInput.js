import React, {Component, PropTypes} from 'react';
import rgba from 'rgba-convert';
import ColorPicker from 'react-simple-colorpicker';

import {colorType, hexStringType} from 'utils/propTypes';

import classes from './ColorPickerInput.scss';

export default class ColorPickerInput extends Component {

  static propTypes = {
    color: PropTypes.oneOfType([colorType, hexStringType]).isRequired,
    setColor: PropTypes.func.isRequired,
  };

  render() {
    const {color, setColor} = this.props;

    return (
      <div className={classes['container']}>
        <ColorPicker
          color={rgba.css(color)}
          onChange={setColor}
        />
      </div>
    );
  }
}
