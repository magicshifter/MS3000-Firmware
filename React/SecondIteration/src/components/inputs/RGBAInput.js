import React, {PropTypes} from 'react';

import {colorType, hexStringType} from 'utils/propTypes';

import ColorInput from 'components/colors/input';

import classes from './RGBAInput.scss';

export const RGBAInput =
  ({color, setColorValue, showAlpha}) =>
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
    </div>;

RGBAInput.propTypes = {
  color: PropTypes.oneOfType([colorType, hexStringType]).isRequired,
  setColorValue: PropTypes.func.isRequired,
  showAlpha: PropTypes.bool,
};

RGBAInput.defaultProps = {
  showAlpha: false,
};

export default RGBAInput;
