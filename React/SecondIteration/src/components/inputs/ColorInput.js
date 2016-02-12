import React, {Component, PropTypes} from 'react';

import NumberInput from 'components/inputs/NumberInput';

import classes from './ColorInput.scss';

export default class ColorInput extends Component {
  static propTypes = {
    val: PropTypes.number.isRequired,
    name: PropTypes.string.isRequired,
    min: PropTypes.number,
    max: PropTypes.number,
    label: PropTypes.string.isRequired,
    setColorValue: PropTypes.func.isRequired,
  };

  render() {
    const {
      val,
      name,
      label,
      setColorValue,
      min = 0,
      max = 255,
    } = this.props;

    return (
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
      </li>
    );
  }
}

