import React, {PropTypes} from 'react';
import rgba from 'rgba-convert';
import {Saturation, Hue} from 'react-color/lib/components/common';

import {colorType, hexStringType} from 'utils/propTypes';

import classes from './ColorPickerInput.scss';

export const ColorPickerInput =
  (props) =>
    <div className={classes['container']}>
      <div className={classes['saturation']}>
        <Saturation
          {...props}
          color={rgba.hex(props.color)}
          direction='vertical'
        />
      </div>
      <div className={classes['hue']}>
        <Hue
          {...props}
          color={rgba.hex(props.color)}
          direction='vertical'
        />
      </div>
    </div>;

ColorPickerInput.propTypes = {
  color: PropTypes.oneOfType([colorType, hexStringType]).isRequired,
};

export default ColorPickerInput;
