import React, {Component, PropTypes} from 'react';
// XXX import rgba from 'rgba-convert';

import {colorType, hexStringType} from 'utils/propTypes';

import ColorInput from './ColorInput';

import classes from './RGBAInput.scss';

export default class RGBAInput extends Component {

  static propTypes = {
    color: PropTypes.oneOfType([colorType, hexStringType]).isRequired,
    setColorValue: PropTypes.func.isRequired,
    showAlpha: PropTypes.bool,
  };

  static defaultProps = {
    showAlpha: false,
  };

  render() {
    const {color, setColorValue, showAlpha} = this.props;

    return (
      <div className={classes['container']}>
         <ul>
          {color && Object.keys(color).map(key => (
            typeof color[key] === 'number' &&
            (key !== 'a' || showAlpha) &&
            <ColorInput
              val={color[key]}
              key={key}
              name={key}
              label={key.toUpperCase()}
              setColorValue={setColorValue}
            />
          ))}
        </ul>
      </div>
    );
  }
}
