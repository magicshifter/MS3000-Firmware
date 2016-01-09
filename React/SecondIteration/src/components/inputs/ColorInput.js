import React, {Component, PropTypes} from 'react';

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
      <li className={classes['input']}>

        {label &&
          <label>{label}</label>
        }

        <button
          onClick={() => setColorValue({name, value: val - 1, min, max})}
        >-</button>

        <input
          type='text'
          name={name}
          value={val}
          size='3'
          onChange={(e) => {
            setColorValue({name, value: parseInt(e.target.value, 10), min, max});
          }}
        />

        <button
          onClick={() => setColorValue({name, value: val + 1, min, max})}
        >+</button>
      </li>
    );
  }
}

