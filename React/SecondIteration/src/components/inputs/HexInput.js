import React, {Component, PropTypes} from 'react';
import rgba from 'rgba-convert';

import {colorType, hexStringType} from 'utils/propTypes';

import ColorInput from './ColorInput';

import classes from './HexInput.scss';

export default class RGBAInput extends Component {

  static propTypes = {
    color: PropTypes.oneOfType([colorType, hexStringType]).isRequired,
    setColor: PropTypes.func.isRequired,
  };

  render() {
    const {color, setColor, showAlpha} = this.props;

    const hex = rgba.css(color);

    return (
      <div className={classes['container']}>
        {/*
        <div
          className={classes['indicator']}
          style={{
            backgroundColor: rgba.css(color),
          }}
        ></div>
        */}
        <label>HEX</label>

        <input
          type='text'
          value={hex}
          setColor={setColor}
        />
      </div>
    );
  }
}
