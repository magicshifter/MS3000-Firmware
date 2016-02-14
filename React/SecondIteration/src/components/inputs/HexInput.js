import React, { Component, PropTypes } from 'react';
import rgba from 'rgba-convert';

import { colorType, hexStringType } from 'utils/propTypes';

import { isHexColor } from 'utils/types';

import classes from './HexInput.scss';

export default class HexInput extends Component {

  static propTypes = {
    color: PropTypes.oneOfType([colorType, hexStringType]).isRequired,
    setColor: PropTypes.func.isRequired,
  };

  constructor(props) {
    super(props);

    this.handleColorChange = this.handleColorChange.bind(this);
    this.cleanHex = this.cleanHex.bind(this);
  }

  handleColorChange(e) {
    const { setColor } = this.props;
    const { value } = e.target;
    const hex = this.cleanHex(value);
    const rgb = rgba.obj(hex);

    this.setState({
      currentColor: hex,
    });

    if (isHexColor(hex)) {
      setColor({
        color: rgb,
      });
    }
  }

  cleanHex(hex) {
    return rgba.hex(hex).substr(0, 7).replace('#', '');
  }

  render() {
    const { currentColor } = this.state;
    const currentHex = this.cleanHex(currentColor);

    return (
      <div className={classes['container']}>

        <label>HEX</label>

        <input
          type='text'
          value={currentHex}
          onChange={this.handleColorChange}
        />
      </div>
    );
  }
}
