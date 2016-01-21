import React, {Component, PropTypes} from 'react';

import {minmax} from 'utils/math';
import {isNumber} from 'utils/types';

import classes from './NumberInput.scss';

export default class NumberInput extends Component {
  static propTypes = {
    val: PropTypes.number.isRequired,
    name: PropTypes.string.isRequired,
    min: PropTypes.number,
    max: PropTypes.number,
    step: PropTypes.number,
    action: PropTypes.func.isRequired,
    label: PropTypes.string,
  };

  static defaultProps = {
    min: 0,
    max: 9999,
    step: 1,
  };

  constructor(props) {
    super(props);

    this.onChange = this.onChange.bind(this);
  }

  onChange(e) {
    const {name, min, max, action} = this.props;

    let value = minmax(parseInt(e.target.value, 10), min, max);

    if (!isNumber(value)) {
      value = 0;
    }

    action({name, value});
  }

  render() {
    const {val, name, label, min, max, step} = this.props;

    return (
      <div className={classes['input']}>

        {label &&
          <label>{label}</label>
        }

        <input
          type='number'
          name={name}
          value={val}
          step={step || 1}
          min={min}
          max={max}
          onChange={this.onChange}
        />
      </div>
    );
  }
}

