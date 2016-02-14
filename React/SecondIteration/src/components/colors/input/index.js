import React, {PropTypes} from 'react';

import NumberInput from 'components/inputs/NumberInput';

import classes from './ColorInput.scss';

export const ColorInput =
  ({val, name, label, setColorValue, min = 0, max = 255}) =>
    <li className={classes['container']}>
      <NumberInput
        type='text'
        label={label}
        name={name}
        val={val}
        min={min}
        max={max}
        action={setColorValue}
      />
    </li>;

ColorInput.propTypes = {
  val: PropTypes.number.isRequired,
  name: PropTypes.string.isRequired,
  min: PropTypes.number,
  max: PropTypes.number,
  label: PropTypes.string.isRequired,
  setColorValue: PropTypes.func.isRequired,
};

export default ColorInput;
